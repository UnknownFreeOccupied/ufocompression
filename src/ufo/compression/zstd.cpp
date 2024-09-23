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

//  UFO
#include <ufo/compression/zstd.hpp>

// ZSTD
#include <zstd.h>

namespace ufo
{
Compressor<CompressionAlgorithm::ZSTD>::Compressor() noexcept
    : compression_level(ZSTD_defaultCLevel())
{
}

Compressor<CompressionAlgorithm::ZSTD>::size_type
Compressor<CompressionAlgorithm::ZSTD>::maxSizeImpl() const
{
	// FIXME: What is the maximum size?
	return std::numeric_limits<size_type>::max();
}

Compressor<CompressionAlgorithm::ZSTD>::size_type Compressor<
    CompressionAlgorithm::ZSTD>::compressBoundImpl(size_type uncompressed_size) const
{
	return ZSTD_compressBound(uncompressed_size);
}

Compressor<CompressionAlgorithm::ZSTD>::size_type
Compressor<CompressionAlgorithm::ZSTD>::compressImpl(std::byte const* src, std::byte* dst,
                                                     size_type src_size,
                                                     size_type dst_cap) const
{
	assert(ZSTD_minCLevel() <= compression_level);
	assert(ZSTD_maxCLevel() >= compression_level);
	return static_cast<size_type>(
	    ZSTD_compress(dst, dst_cap, src, src_size, compression_level));
}

Compressor<CompressionAlgorithm::ZSTD>::size_type
Compressor<CompressionAlgorithm::ZSTD>::decompressImpl(std::byte const* src,
                                                       std::byte* dst, size_type src_size,
                                                       size_type dst_cap) const
{
	return static_cast<size_type>(ZSTD_decompress(dst, dst_cap, src, src_size));
}
}  // namespace ufo