#include "../src/data_compression_lib/data_compression.hpp"
#include "catch2/catch_test_macros.hpp"

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
