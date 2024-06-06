#include "../src/data_compression_lib/data_compression.h"
#include "catch2/catch_test_macros.hpp"

TEST_CASE("Huffman encoding and decoding", "[huffman]") {
	std::string text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Morbi a "
	                   "sagittis orci, sed tincidunt erat. Pellentesque neque risus, scelerisque "
	                   "sed eros in, cursus ultrices neque. Donec sollicitudin magna non felis "
	                   "vulputate, at sollicitudin felis dignissim. Pellentesque dapibus aliquet "
	                   "pretium. Aliquam pellentesque faucibus mauris at ullamcorper. Aenean auctor, "
	                   "sapien a malesuada suscipit, dui magna tincidunt orci, vitae bibendum sem "
	                   "nunc sit amet libero. Duis ullamcorper arcu dolor, quis ultricies magna "
	                   "efficitur in. Quisque maximus egestas tellus, quis aliquet lorem "
	                   "sollicitudin a. Etiam fringilla nisl felis, in efficitur lacus viverra ut. "
	                   "Maecenas nibh elit, fermentum nec nisi ac, egestas tincidunt nibh.";
	std::unordered_map<char, std::pair<uint32_t, int>> huffmanCode;

	std::vector<uint8_t> compressedText = compress(text, huffmanCode);

	REQUIRE(!compressedText.empty());

	REQUIRE(!huffmanCode.empty());

	std::string decompressedText = decompress(compressedText, huffmanCode);

	REQUIRE(decompressedText == text);
}