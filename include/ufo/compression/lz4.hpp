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

#ifndef UFO_COMPRESSION_LZ4_HPP
#define UFO_COMPRESSION_LZ4_HPP

// UFO
#include <ufo/compression/algorithm.hpp>
#include <ufo/compression/compressor.hpp>

// STL
#include <cstddef>

// LZ4 compression
#include <lz4.h>
#include <lz4hc.h>

namespace ufo
{
template <>
struct Compressor<CompressionAlgorithm::LZ4> : public CompressorBase {
	int acceleration      = 1;
	int compression_level = 0;

	Compressor() noexcept         = default;
	Compressor(Compressor const&) = default;
	Compressor(Compressor&&)      = default;

	Compressor(int acceleration, int compression_level = 0)
	    : acceleration(acceleration), compression_level(compression_level)
	{
	}

	~Compressor() override = default;

	Compressor& operator=(Compressor const&) = default;
	Compressor& operator=(Compressor&&)      = default;

	[[nodiscard]] CompressionAlgorithm type() const noexcept override
	{
		return CompressionAlgorithm::LZ4;
	}

	// TODO: Maybe add "raw" methods that can be used to read for example PCL point clouds

 protected:
	[[nodiscard]] size_type maxSizeImpl() const override
	{
		return static_cast<size_type>(LZ4_MAX_INPUT_SIZE);
	}

	[[nodiscard]] size_type compressBoundImpl(size_type uncompressed_size) const override
	{
		return static_cast<size_type>(LZ4_compressBound(LZ4_MAX_INPUT_SIZE));
	}

	size_type compressImpl(std::byte const* src, std::byte* dst, size_type src_size,
	                       size_type dst_cap) const override
	{
		return static_cast<size_type>(
		    0 < compression_level
		        ? LZ4_compress_HC(reinterpret_cast<char const*>(src),
		                          reinterpret_cast<char*>(dst), static_cast<int>(src_size),
		                          static_cast<int>(dst_cap), compression_level)
		        : LZ4_compress_fast(reinterpret_cast<char const*>(src),
		                            reinterpret_cast<char*>(dst), static_cast<int>(src_size),
		                            static_cast<int>(dst_cap), acceleration));
	}

	[[nodiscard]] size_type decompressImpl(std::byte const* src, std::byte* dst,
	                                       size_type src_size,
	                                       size_type dst_cap) const override
	{
		return static_cast<std::size_t>(LZ4_decompress_safe(
		    reinterpret_cast<char const*>(src), reinterpret_cast<char*>(dst),
		    static_cast<int>(src_size), static_cast<int>(dst_cap)));
	}

	[[nodiscard]] Compressor* clone() const override { return new Compressor(*this); }
};
}  // namespace ufo

#endif  // UFO_COMPRESSION_LZ4_HPP