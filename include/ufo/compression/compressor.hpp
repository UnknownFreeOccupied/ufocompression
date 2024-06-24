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
#include <iostream>
#include <limits>
#include <memory>
#include <vector>

namespace ufo
{
template <CompressionAlgorithm Algorithm>
struct Compressor;

struct CompressorBase {
	using size_type = std::size_t;

	CompressorBase() noexcept = default;

	CompressorBase(CompressorBase const& other)
	{
		if (other.next_) {
			next_.reset(other.next_->clone());
		}
	}

	CompressorBase(CompressorBase&&) = default;

	virtual ~CompressorBase() = default;

	CompressorBase& operator=(CompressorBase const& rhs)
	{
		if (rhs.next_) {
			next_.reset(rhs.clone());
		}
		return *this;
	}

	CompressorBase& operator=(CompressorBase&&) = default;

	[[nodiscard]] size_type size() const
	{
		size_type ret = 1;
		for (auto it = &next(); it; it = &it->next()) {
			++ret;
		}
		return ret;
	}

	[[nodiscard]] CompressorBase& next() { return *next_; }

	[[nodiscard]] CompressorBase const& next() const { return *next_; }

	template <CompressionAlgorithm Algorithm>
	CompressorBase& next(Compressor<Algorithm> const& next)
	{
		next_ = std::make_unique<Compressor<Algorithm>>(next);
		return *next_;
	}

	template <CompressionAlgorithm Algorithm>
	CompressorBase& next(Compressor<Algorithm>&& next)
	{
		next_ = std::make_unique<Compressor<Algorithm>>(std::move(next));
		return *next_;
	}

	[[nodiscard]] virtual CompressionAlgorithm type() const noexcept = 0;

	[[nodiscard]] std::vector<CompressionAlgorithm> chain() const
	{
		std::vector<CompressionAlgorithm> chain;
		chain.reserve(size());
		chain.push_back(type());
		for (auto it = &next(); it; it = &it->next()) {
			chain.push_back(it->type());
		}
		return chain;
	}

	[[nodiscard]] size_type maxSize() const
	{
		return std::numeric_limits<size_type>::max();
	}

	[[nodiscard]] size_type maxSizeNative() const
	{
		// FIXME: Can be incorrect since compressed can be larger than uncompressed

		auto ms = maxSizeImpl();
		for (auto it = &next(); it; it = &it->next()) {
			ms = std::min(ms, it->maxSizeImpl());
		}
		return ms;
	}

	[[nodiscard]] size_type compressBound(size_type uncompressed_size) const
	{
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

	[[nodiscard]] size_type compressBoundNative(size_type uncompressed_size) const
	{
		auto bound = compressBoundImpl(uncompressed_size);
		for (auto it = &next(); it; it = &it->next()) {
			bound = it->compressBoundImpl(bound);
		}
		return bound;
	}

	size_type compress(std::istream& in, std::ostream& out,
	                   size_type uncompressed_size) const
	{
		// TODO: Implement
		return 0;

		// std::uint64_t size = static_cast<std::uint64_t>(uncompressed_size);

		// auto compressed_size = compressBound(uncompressed_size);
		// auto src             = std::make_unique<char[]>(uncompressed_size + sizeof size);
		// auto dst            = std::make_unique<char[]>(compressed_size);

		// std::memcpy(src.get(), &size, sizeof size);

		// in.read(src.get() + sizeof size, uncompressed_size);

		// compressed_size = compress(src, dst);

		// // TODO: Implement
		// for (auto it = next(); it; it = it->next()) {
		// 	std::swap(src, dst);
		// 	compressed_size = it->compress(src, dst);
		// }

		// // TODO: Implement
		// out.write(dst.get(), compressed_size);

		// return compressed_size;
	}

	size_type compress(ReadBuffer& in, WriteBuffer& out) const
	{
		// TODO: Implement
		return 0;
	}

	size_type compressNative(std::istream& in, std::ostream& out,
	                         size_type uncompressed_size) const
	{
		// TODO: Implement
		return 0;
	}

	size_type compressNative(ReadBuffer& in, WriteBuffer& out) const
	{
		// TODO: Implement
		return 0;
	}

	size_type decompress(std::istream& in, std::ostream& out) const
	{
		// TODO: Implement
		return 0;
	}

	size_type decompress(ReadBuffer& in, WriteBuffer& out) const
	{
		// TODO: Implement
		return 0;
	}

	size_type decompressNative(std::istream& in, std::ostream& out) const
	{
		// TODO: Implement
		return 0;
	}

	size_type decompressNative(ReadBuffer& in, WriteBuffer& out) const
	{
		// TODO: Implement
		return 0;
	}

 protected:
	[[nodiscard]] virtual size_type maxSizeImpl() const = 0;

	[[nodiscard]] virtual size_type compressBoundImpl(
	    size_type uncompressed_size) const = 0;

	virtual size_type compressImpl(std::byte const* src, std::byte* dst, size_type src_size,
	                               size_type dst_cap) const = 0;

	virtual size_type decompressImpl(std::byte const* src, std::byte* dst,
	                                 size_type src_size, size_type dst_cap) const = 0;

	[[nodiscard]] virtual CompressorBase* clone() const = 0;

 private:
	std::unique_ptr<CompressorBase> next_;
};
}  // namespace ufo

#endif  // UFO_COMPRESSION_COMPRESSOR_HPP