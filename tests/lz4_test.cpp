// UFO
#include <ufo/compression/compression.hpp>

// Catch2
#include <catch2/catch_test_macros.hpp>

// STL
#include <filesystem>
#include <fstream>

TEST_CASE("NONE Compression") { ufo::CompressorNone compressor; }

TEST_CASE("LZ4 Compression")
{
	ufo::CompressorLZ4 compressor;
	compressor.next(ufo::CompressorZSTD())
	    .next(ufo::CompressorZLIB())
	    .next(ufo::CompressorLZF());

	for (ufo::CompressorBase* it = &compressor; it; it = &it->next()) {
		std::cout << ufo::enumToString(it->type()) << std::endl;
	}

	std::cout << compressor.size() << std::endl;

	std::ifstream in(std::filesystem::path("/home/dduberg/Desktop/ufo.svg"),
	                 std::ios::binary);
	std::ofstream out(std::filesystem::path("/home/dduberg/Desktop/ufo_compressed.svg"),
	                  std::ios::binary);

	in.seekg(0, std::ios::seekdir::_S_end);

	std::size_t uncompressed_size = in.tellg();

	in.seekg(0, std::ios::seekdir::_S_beg);

	compressor.compress(in, out, uncompressed_size);

	in.close();
	out.close();

	// ufo::Compression::decompress(in, out);
}

TEST_CASE("ZSTD Compression") { ufo::CompressorZSTD compressor; }

TEST_CASE("LZF Compression") { ufo::CompressorLZF compressor; }

TEST_CASE("ZLIB Compression") { ufo::CompressorZLIB compressor; }