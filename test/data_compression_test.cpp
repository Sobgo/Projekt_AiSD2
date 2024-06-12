#include "../src/data_compression_lib/data_compression.hpp"
#include "catch2/catch_test_macros.hpp"
#include <random>
namespace compression_test {
std::string generate_random_string(size_t length, size_t alphabet_size);
}

TEST_CASE("Huffman encoding and decoding", "[huffman]") {
	std::string text =
	    "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Morbi a "
	    "sagittis orci, sed tincidunt erat. Pellentesque neque risus, scelerisque "
	    "sed eros in, cursus ultrices neque. Donec sollicitudin magna non felis "
	    "vulputate, at sollicitudin felis dignissim. Pellentesque dapibus aliquet "
	    "pretium. Aliquam pellentesque faucibus mauris at ullamcorper. Aenean auctor, "
	    "sapien a malesuada suscipit, dui magna tincidunt orci, vitae bibendum sem "
	    "nunc sit amet libero. Duis ullamcorper arcu dolor, quis ultricies magna "
	    "efficitur in. Quisque maximus egestas tellus, quis aliquet lorem "
	    "sollicitudin a. Etiam fringilla nisl felis, in efficitur lacus viverra ut. "
	    "Maecenas nibh elit, fermentum nec nisi ac, egestas tincidunt nibh.";
	auto huffmanCode = data_compression::generate_huffman_code(text);

	auto compressedText = data_compression::compress(text, huffmanCode);

	std::string decompressedText = data_compression::decompress(compressedText, huffmanCode);

	REQUIRE(decompressedText == text);
}

TEST_CASE("Empty text compression and decompression", "[huffman]") {
	std::string text = "";
	auto huffmanCode = data_compression::generate_huffman_code(text);
	auto compressedText = data_compression::compress(text, huffmanCode);
	std::string decompressedText = data_compression::decompress(compressedText, huffmanCode);
	REQUIRE(decompressedText == text);
}

TEST_CASE("Single character text compression and decompression", "[huffman]") {
	std::string text = "A";
	auto huffmanCode = data_compression::generate_huffman_code(text);
	auto compressedText = data_compression::compress(text, huffmanCode);
	std::string decompressedText = data_compression::decompress(compressedText, huffmanCode);
	REQUIRE(decompressedText == text);
}

TEST_CASE("Repeated character text compression and decompression", "[huffman]") {
	std::string text = "AAAAAABBBBC";
	auto huffmanCode = data_compression::generate_huffman_code(text);
	auto compressedText = data_compression::compress(text, huffmanCode);
	std::string decompressedText = data_compression::decompress(compressedText, huffmanCode);
	REQUIRE(decompressedText == text);
}

TEST_CASE("Randomized text compression and decompression", "[huffman]") {
	for (size_t i = 0; i < 100; ++i) {
		std::string text = compression_test::generate_random_string(100, i % 26 + 1);

		auto huffmanCode = data_compression::generate_huffman_code(text);
		auto compressedText = data_compression::compress(text, huffmanCode);
		std::string decompressedText = data_compression::decompress(compressedText, huffmanCode);
		REQUIRE(decompressedText == text);
	}
}

namespace compression_test {

std::string generate_random_string(size_t length, size_t alphabet_size) {
	std::string s = "";

	std::uniform_int_distribution<int> dis(0, alphabet_size - 1);
	std::default_random_engine gen(time(NULL));

	for (size_t i = 0; i < length; ++i) {
		s.push_back('a' + dis(gen));
	}

	return s;
}

}
