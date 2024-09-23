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

#ifndef UFO_COMPRESSION_ZLIB_HPP
#define UFO_COMPRESSION_ZLIB_HPP

// UFO
#include <ufo/compression/algorithm.hpp>
#include <ufo/compression/compressor.hpp>
#include <ufo/utility/io/buffer.hpp>

// STL
#include <cstddef>

namespace ufo
{
struct CompressorZLIB : public Compressor {
	int compression_level;

	CompressorZLIB() noexcept;
	CompressorZLIB(CompressorZLIB const&) = default;
	CompressorZLIB(CompressorZLIB&&)      = default;

	CompressorZLIB(int compression_level) : compression_level(compression_level) {}

	~CompressorZLIB() override = default;

	CompressorZLIB& operator=(CompressorZLIB const&) = default;
	CompressorZLIB& operator=(CompressorZLIB&&)      = default;

	[[nodiscard]] CompressionAlgorithm type() const noexcept override
	{
		return CompressionAlgorithm::ZLIB;
	}

	using Compressor::compress;
	using Compressor::decompress;

 protected:
	[[nodiscard]] size_type maxSizeImpl() const override;

	[[nodiscard]] size_type compressBoundImpl(size_type uncompressed_size) const override;

	size_type compress(std::byte const* src, std::byte* dst, size_type src_size,
	                   size_type dst_cap) const override;

	size_type decompress(std::byte const* src, std::byte* dst, size_type src_size,
	                     size_type dst_cap) const override;

	[[nodiscard]] CompressorZLIB* clone() const override
	{
		return new CompressorZLIB(*this);
	}
};
}  // namespace ufo

#endif  // UFO_COMPRESSION_ZLIB_HPP