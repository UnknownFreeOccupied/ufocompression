// UFO
#include <ufo/compression/compression.hpp>

// Catch2
#include <catch2/catch_test_macros.hpp>

// STL
#include <filesystem>
#include <fstream>

using namespace ufo;

TEST_CASE("NONE Compression") { CompressorNone compressor; }

TEST_CASE("LZ4 Compression")
{
	CompressorLZ4 compressor;
	// compressor.next(CompressorZSTD()).next(CompressorZLIB()).next(CompressorLZF());

	for (Compressor* it = &compressor; it; it = &it->next()) {
		std::cout << enumToString(it->type()) << std::endl;
	}

	std::cout << compressor.size() << std::endl;

	SECTION("Compress")
	{
		std::ifstream in(std::filesystem::path("/home/dduberg/Desktop/UFOMap.zip"),
		                 std::ios::binary);
		std::ofstream out(
		    std::filesystem::path("/home/dduberg/Desktop/UFOMap_compressed.zip"),
		    std::ios::binary);

		in.seekg(0, std::ios::seekdir::_S_end);

		std::size_t uncompressed_size = in.tellg();

		in.seekg(0, std::ios::seekdir::_S_beg);

		// compressor.compress(in, out, uncompressed_size, true);

		in.close();
		out.close();

		compressor.compress("/home/dduberg/ufo/ufotime", "");
	}

	SECTION("Decompress")
	{
		// std::ifstream
		// in(std::filesystem::path("/home/dduberg/Desktop/UFOMap_compressed.zip"),
		//                  std::ios::binary);
		// std::ofstream out(std::filesystem::path("/home/dduberg/Desktop/UFOMap2.zip"),
		//                   std::ios::binary);

		// compressor.decompress(in, out, true);
	}
}

TEST_CASE("ZSTD Compression") { CompressorZSTD compressor; }

TEST_CASE("LZF Compression") { CompressorLZF compressor; }

TEST_CASE("ZLIB Compression") { CompressorZLIB compressor; }