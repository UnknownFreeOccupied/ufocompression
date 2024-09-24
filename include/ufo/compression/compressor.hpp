/*!
 * UFOMap: An Efficient Probabilistic 3D Mapping Framework That Embraces the Unknown
 *
 * @author Daniel Duberg (dduberg@kth.se)
 * @see https://github.com/UnknownFreeOccupied/ufomap
 * @version 1.0
 * @date 2022-05-13
 *
 * @copyright Copyright (c) 2022, Daniel Duberg, KTH Royal Institute of Technology
 *
 * BSD 3-Clause License
 *
 * Copyright (c) 2022, Daniel Duberg, KTH Royal Institute of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *     list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef UFO_COMPRESSION_COMPRESSOR_HPP
#define UFO_COMPRESSION_COMPRESSOR_HPP

// UFO
#include <ufo/compression/algorithm.hpp>
#include <ufo/utility/io/buffer.hpp>

// STL
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <limits>
#include <memory>
#include <type_traits>
#include <vector>

namespace ufo
{
// Forward declare
struct Compressor;

// Type traits
template <class T>
using is_compressor = std::is_base_of<Compressor, T>;

template <class T>
inline constexpr bool is_compressor_v = is_compressor<T>::value;

// Implement
struct Compressor {
	using size_type = std::uintmax_t;

	Compressor() noexcept = default;

	Compressor(Compressor const& other)
	{
		if (other.next_) {
			next_.reset(other.next_->clone());
		}
	}

	Compressor(Compressor&&) = default;

	virtual ~Compressor() = default;

	Compressor& operator=(Compressor const& rhs)
	{
		if (rhs.next_) {
			next_.reset(rhs.clone());
		}
		return *this;
	}

	Compressor& operator=(Compressor&&) = default;

	[[nodiscard]] size_type size() const
	{
		size_type ret = 1;
		for (auto it = &next(); it; it = &it->next()) {
			++ret;
		}
		return ret;
	}

	[[nodiscard]] Compressor& next() { return *next_; }

	[[nodiscard]] Compressor const& next() const { return *next_; }

	template <class Comp, std::enable_if_t<is_compressor_v<Comp>, bool> = true>
	Compressor& next(Comp const& next)
	{
		next_ = std::make_unique<Comp>(next);
		return *next_;
	}

	template <class Comp, std::enable_if_t<is_compressor_v<Comp>, bool> = true>
	Compressor& next(Comp&& next)
	{
		next_ = std::make_unique<Comp>(std::forward<Comp>(next));
		return *next_;
	}

	[[nodiscard]] virtual CompressionAlgorithm type() const noexcept = 0;

	[[nodiscard]] std::vector<CompressionAlgorithm> typeChain() const
	{
		std::vector<CompressionAlgorithm> chain;
		chain.reserve(size());
		chain.push_back(type());
		for (auto it = &next(); it; it = &it->next()) {
			chain.push_back(it->type());
		}
		return chain;
	}

	[[nodiscard]] std::vector<Compressor*> chain()
	{
		std::vector<Compressor*> chain;
		chain.reserve(size());
		chain.push_back(this);
		for (auto it = &next(); it; it = &it->next()) {
			chain.push_back(it);
		}
		return chain;
	}

	[[nodiscard]] std::vector<Compressor const*> chain() const
	{
		std::vector<Compressor const*> chain;
		chain.reserve(size());
		chain.push_back(this);
		for (auto it = &next(); it; it = &it->next()) {
			chain.push_back(it);
		}
		return chain;
	}

	[[nodiscard]] size_type maxSize(bool native = false) const
	{
		if (!native) {
			return std::numeric_limits<size_type>::max();
		}

		// FIXME: Can be incorrect since compressed can be larger than uncompressed

		auto ms = maxSizeImpl();
		for (auto it = &next(); it; it = &it->next()) {
			ms = std::min(ms, it->maxSizeImpl());
		}
		return ms;
	}

	[[nodiscard]] size_type compressBound(size_type uncompressed_size,
	                                      bool      native = false) const
	{
		if (native) {
			auto bound = compressBoundImpl(uncompressed_size);
			for (auto it = &next(); it; it = &it->next()) {
				bound = it->compressBoundImpl(bound);
			}
			return bound;
		} else {
			size_type bound = uncompressed_size;
			for (auto it = this; it; it = &it->next()) {
				auto ms = it->maxSizeImpl();
				auto a  = bound / ms;
				auto b  = bound % ms;

				bound = a * it->compressBoundImpl(ms) + it->compressBoundImpl(b) +
				        (a + 2) * sizeof(std::uint64_t);
			}

			// Add cost of chain
			return (size() + 1) * sizeof(CompressionAlgorithm) + bound;
		}
	}

	size_type compress(std::filesystem::path const& in,
	                   std::filesystem::path const& out) const
	{
		size_type   uncompressed_size{};
		std::size_t num{};
		if (std::filesystem::is_directory(in)) {
			for (auto const& entry : std::filesystem::recursive_directory_iterator(in)) {
				if (entry.is_regular_file()) {
					++num;
					uncompressed_size += std::filesystem::file_size(entry.path());
				} else if (entry.is_directory()) {
					++num;
				}
			}
		}

		std::cout << "Num: " << num << '\n';
		std::cout << "Size: " << uncompressed_size << '\n';

		return 0;
	}

	size_type compress(std::istream& in, std::ostream& out, size_type uncompressed_size,
	                   bool native = false) const
	{
		auto compressed_bound = compressBound(uncompressed_size, native);

		auto buffer_size = std::max(uncompressed_size, compressed_bound);

		if (native) {
			auto src = std::make_unique<std::byte[]>(buffer_size);
			auto dst = std::make_unique<std::byte[]>(buffer_size);

			in.read(reinterpret_cast<char*>(dst.get()), uncompressed_size);

			auto compressed_size = uncompressed_size;
			for (auto it = this; it; it = &it->next()) {
				std::swap(src, dst);
				compressed_size = compress(src.get(), dst.get(), compressed_size, buffer_size);
			}

			out.write(reinterpret_cast<char const*>(dst.get()), compressed_size);

			return compressed_size;
		}

		return 0;

		// std::uint64_t size = static_cast<std::uint64_t>(uncompressed_size);

		// buffer_size += sizeof(size);

		// auto src = std::make_unique<std::byte[]>(buffer_size);
		// auto dst = std::make_unique<std::byte[]>(buffer_size);

		// // std::memcpy(src.get(), &size, sizeof(size));

		// // in.read(reinterpret_cast<char*>(src.get() + sizeof(size)), uncompressed_size);

		// // compressed_size =
		// //     compressImpl(src.get(), dst.get(), uncompressed_size, compressed_size);

		// // // TODO: Implement
		// // for (auto const* it = &next(); it; it = &(it->next())) {
		// // 	std::swap(src, dst);
		// // 	compressed_size =
		// // 	    it->compressImpl(src.get(), dst.get(), compressed_size, buffer_size);
		// // }

		// // TODO: Implement
		// out.write(reinterpret_cast<char const*>(dst.get()), compressed_size);

		// return compressed_size;
	}

	size_type compress(ReadBuffer& in, WriteBuffer& out, bool native = false) const
	{
		// TODO: Implement
		return 0;
	}

	static size_type decompress(std::istream& in, std::ostream& out)
	{
		// TODO: Implement
		return 0;
	}

	static size_type decompress(ReadBuffer& in, WriteBuffer& out)
	{
		// TODO: Implement
		return 0;
	}

	size_type decompress(std::istream& in, std::ostream& out, bool native) const
	{
		if (!native) {
			return decompress(in, out);
		}

		in.seekg(0, std::ios::seekdir::_S_end);

		std::size_t uncompressed_size = in.tellg();

		in.seekg(0, std::ios::seekdir::_S_beg);

		// TODO: Implement
		return 0;
	}

	size_type decompress(ReadBuffer& in, WriteBuffer& out, bool native) const
	{
		if (!native) {
			return decompress(in, out);
		}

		// TODO: Implement
		return 0;
	}

 protected:
	[[nodiscard]] virtual size_type maxSizeImpl() const = 0;

	[[nodiscard]] virtual size_type compressBoundImpl(
	    size_type uncompressed_size) const = 0;

	virtual size_type compress(std::byte const* src, std::byte* dst, size_type src_size,
	                           size_type dst_cap) const = 0;

	virtual size_type decompress(std::byte const* src, std::byte* dst, size_type src_size,
	                             size_type dst_cap) const = 0;

	[[nodiscard]] virtual Compressor* clone() const = 0;

 private:
	std::unique_ptr<Compressor> next_;
};
}  // namespace ufo

#endif  // UFO_COMPRESSION_COMPRESSOR_HPP