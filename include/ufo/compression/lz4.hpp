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

namespace ufo
{
struct CompressorLZ4 : public Compressor {
	int acceleration      = 1;
	int compression_level = 0;

	CompressorLZ4() noexcept            = default;
	CompressorLZ4(CompressorLZ4 const&) = default;
	CompressorLZ4(CompressorLZ4&&)      = default;

	CompressorLZ4(int acceleration, int compression_level = 0)
	    : acceleration(acceleration), compression_level(compression_level)
	{
	}

	~CompressorLZ4() override = default;

	CompressorLZ4& operator=(CompressorLZ4 const&) = default;
	CompressorLZ4& operator=(CompressorLZ4&&)      = default;

	[[nodiscard]] CompressionAlgorithm type() const noexcept override
	{
		return CompressionAlgorithm::LZ4;
	}

	// TODO: Maybe add "raw" methods that can be used to read for example PCL point clouds

	using Compressor::compress;
	using Compressor::decompress;

 protected:
	[[nodiscard]] size_type maxSizeImpl() const override;

	[[nodiscard]] size_type compressBoundImpl(size_type uncompressed_size) const override;

	size_type compress(std::byte const* src, std::byte* dst, size_type src_size,
	                   size_type dst_cap) const override;

	[[nodiscard]] size_type decompress(std::byte const* src, std::byte* dst,
	                                   size_type src_size,
	                                   size_type dst_cap) const override;

	[[nodiscard]] CompressorLZ4* clone() const override { return new CompressorLZ4(*this); }
};
}  // namespace ufo

#endif  // UFO_COMPRESSION_LZ4_HPP