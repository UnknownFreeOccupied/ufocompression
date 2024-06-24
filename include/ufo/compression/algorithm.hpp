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

#ifndef UFO_COMPRESSION_ALGORITHM_HPP
#define UFO_COMPRESSION_ALGORITHM_HPP

// STL
#include <cstdint>
#include <string_view>

namespace ufo
{
enum class CompressionAlgorithm : std::uint32_t {
	NONE = 0,
	LZ4  = 1,
	ZSTD = 2,
	LZF  = 3,
	ZLIB = 4
};

template <CompressionAlgorithm CAlg>
[[nodiscard]] constexpr std::string_view enumToString()
{
	using namespace std::literals;
	if constexpr (CompressionAlgorithm::NONE == CAlg) {
		return "none"sv;
	} else if constexpr (CompressionAlgorithm::LZ4 == CAlg) {
		return "lz4"sv;
	} else if constexpr (CompressionAlgorithm::ZSTD == CAlg) {
		return "zstd"sv;
	} else if constexpr (CompressionAlgorithm::LZF == CAlg) {
		return "lzf"sv;
	} else if constexpr (CompressionAlgorithm::ZLIB == CAlg) {
		return "zlib"sv;
	} else {
		// Error
	}
}

[[nodiscard]] constexpr std::string_view enumToString(CompressionAlgorithm PC)
{
	switch (PC) {
		case CompressionAlgorithm::NONE: return enumToString<CompressionAlgorithm::NONE>();
		case CompressionAlgorithm::LZ4: return enumToString<CompressionAlgorithm::LZ4>();
		case CompressionAlgorithm::ZSTD: return enumToString<CompressionAlgorithm::ZSTD>();
		case CompressionAlgorithm::LZF: return enumToString<CompressionAlgorithm::LZF>();
		case CompressionAlgorithm::ZLIB: return enumToString<CompressionAlgorithm::ZLIB>();
	}
	// Error
	return "";
}
}  // namespace ufo

#endif  // UFO_COMPRESSION_ALGORITHM_HPP