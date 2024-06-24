// UFO
#include <ufo/compression/compression.hpp>

// Catch2
#include <catch2/catch_test_macros.hpp>

TEST_CASE("NONE Compression") { ufo::CompressorNone compressor; }

TEST_CASE("LZ4 Compression")
{
	ufo::CompressorLZ4 compressor;
	compressor.next(ufo::CompressorZSTD()).next(ufo::CompressorZLIB());

	for (ufo::CompressorBase* it = &compressor; it; it = &it->next()) {
		std::cout << ufo::enumToString(it->type()) << std::endl;
	}

	std::cout << compressor.size() << std::endl;
}

TEST_CASE("ZSTD Compression") { ufo::CompressorZSTD compressor; }

TEST_CASE("LZF Compression") { ufo::CompressorLZF compressor; }

TEST_CASE("ZLIB Compression") { ufo::CompressorZLIB compressor; }