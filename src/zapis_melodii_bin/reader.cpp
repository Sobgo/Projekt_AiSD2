#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

#include "../data_compression_lib/data_compression.hpp"

using namespace std;

int int_log2(int n) {
	int result = 0;
	while (n >>= 1) {
		result++;
	}
	return result;
}

int main(int argc, char *argv[]) {
	istream *instream = nullptr;
	ostream *outstream = nullptr;

	ifstream infile;
	ofstream outfile;

	// NOLINTBEGIN(cppcoreguidelines-pro-bounds-pointer-arithmetic)
	if (argc < 2) {
		cerr << "Usage: " << argv[0] << " <input file> <output file>\n";
		return 1;
	}
	if (strcmp(argv[1], "--") == 0) {
		instream = &cin;
	} else {
		infile.open(argv[1]);
		if (!infile) {
			cerr << "Error: could not open input file\n";
			return 1;
		}
		instream = &infile;
	}
	if (strcmp(argv[2], "--") == 0) {
		outstream = &cout;
	} else {
		outfile.open(argv[2]);
		if (!outfile) {
			cerr << "Error: could not open output file\n";
			return 1;
		}
		outstream = &outfile;
	}
	// NOLINTEND(cppcoreguidelines-pro-bounds-pointer-arithmetic)

	vector<bool> data;
	for (istreambuf_iterator<char> it(*instream); it != istreambuf_iterator<char>(); ++it) {
		if (*it == '0') {
			data.push_back(false);
		} else if (*it == '1') {
			data.push_back(true);
		}
	}
	if (data.size() < 16) {
		cerr << "Error: input file too short\n";
		return 1;
	}
	size_t current_idx = 0;

	uint8_t num_chars = 0;
	for (int i = 0; i < 8; i++) {
		num_chars <<= 1;
		num_chars |= data[current_idx++];
	}
	const uint8_t max_bits_to_encode_num_chars = int_log2(num_chars) + 1;
	int max_bitlen = 0;
	for (int i = 0; i < 8; i++) {
		max_bitlen <<= 1;
		max_bitlen |= data[current_idx++];
	}
	vector<int> codebook_char_num_per_bitlen(max_bitlen, 0);
	for (int i = 0; i < max_bitlen; i++) {
		for (int j = 0; j < max_bits_to_encode_num_chars; j++) {
			codebook_char_num_per_bitlen[i] <<= 1;
			codebook_char_num_per_bitlen[i] |= data[current_idx++];
		}
	}
	vector<char> codebook_chars(num_chars);
	for (int i = 0; i < num_chars; i++) {
		for (int j = 0; j < 7; j++) {
			codebook_chars[i] <<= 1;
			codebook_chars[i] |= data[current_idx++];
		}
	}
	data_compression::HuffmanCode huffman_code;
	{
		vector<pair<char, int>> char_codelengths(num_chars);
		int current_len_idx = 0;
		int remaining_of_len = codebook_char_num_per_bitlen[current_len_idx];
		for (int i = 0; i < num_chars; i++) {
			while (remaining_of_len == 0) {
				current_len_idx++;
				remaining_of_len = codebook_char_num_per_bitlen[current_len_idx];
			}
			char_codelengths[i] = {codebook_chars[i], current_len_idx + 1};
			remaining_of_len--;
		}

		uint32_t code = 0;
		int prevLen = 0;
		for (auto &pair : char_codelengths) {
			const int len = pair.second;
			if (prevLen == 0) {
				huffman_code[pair.first] = {code, len};
				prevLen = len;
				continue;
			}
			code++;
			code <<= (len - prevLen);
			huffman_code[pair.first] = {code, len};
			prevLen = len;
		}
	}
	data.erase(data.begin(), data.begin() + current_idx);
	const string decoded_text = data_compression::decompress(data, huffman_code);
	*outstream << decoded_text;
}
