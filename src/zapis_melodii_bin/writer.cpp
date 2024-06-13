#include <algorithm>
#include <bitset>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <sstream>
#include <utility>
#include <vector>

#include "../data_compression_lib/data_compression.hpp"
#include "../pattern_matching_lib/pattern_matching.hpp"

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

	ifstream replacementsfile;
	ifstream infile;
	ofstream outfile;

	// NOLINTBEGIN(cppcoreguidelines-pro-bounds-pointer-arithmetic)
	if (argc < 4) {
		cerr << "Usage: " << argv[0] << " <replacements file> <input file> <output file>\n";
		return 1;
	}

	replacementsfile.open(argv[1]);
	if (!replacementsfile) {
		cerr << "Error: could not open input file\n";
		return 1;
	}

	if (strcmp(argv[2], "--") == 0) {
		instream = &cin;
	} else {
		infile.open(argv[2]);
		if (!infile) {
			cerr << "Error: could not open output file\n";
			return 1;
		}
		instream = &infile;
	}

	if (strcmp(argv[3], "--") == 0) {
		outstream = &cout;
	} else {
		outfile.open(argv[3]);
		if (!outfile) {
			cerr << "Error: could not open output file\n";
			return 1;
		}
		outstream = &outfile;
	}
	// NOLINTEND(cppcoreguidelines-pro-bounds-pointer-arithmetic)

	string replaced_text;
	{
		vector<pair<string, string>> replacements;
		vector<string> patterns;
		string line;
		set<char> alphabet_set = {'\x7F'};
		while (getline(replacementsfile, line)) {
			istringstream ss(line);
			string from, to;
			ss >> from >> to;
			replacements.emplace_back('\x7F' + std::move(from) + '\x7F',
			                          '\x7F' + std::move(to) + '\x7F');
			patterns.push_back(replacements.back().first);
			for (const char &c : replacements.back().first) {
				alphabet_set.insert(c);
			}
			for (const char &c : replacements.back().second) {
				alphabet_set.insert(c);
			}
		}

		string input("\x7F");
		string alphabet;

		for (istreambuf_iterator<char> it(*instream); it != istreambuf_iterator<char>(); ++it) {
			const char c = *it;
			if (c == '\r') {
				continue;
			}
			if (c == '\n') {
				input.append("\x7F\n\x7F");
			} else if (c == ' ') {
				input.append("\x7F \x7F");
			} else {
				input += c;
			}
			alphabet_set.insert(c);
		}
		input += '\x7F';
		alphabet.assign(alphabet_set.begin(), alphabet_set.end());

		auto matching_result = pattern_matching::aho_corasick(alphabet, input, patterns);
		for (size_t i = 0; i < matching_result.size(); ++i) {
			const auto &matches = matching_result[i];
			for (size_t j = 0; j < matches.size(); ++j) {
				const auto &match = matches[j];
				const auto &replacement = replacements[i];
				input.replace(match, replacement.first.size(), replacement.second);
			}
		}
		for (const char &c : input) {
			if (c == '\x7F') {
				continue;
			}
			replaced_text += c;
		}
	}

	vector<bool> codebook_bits;
	vector<bool> message_bits;
	{
		auto huffman_code = data_compression::generate_huffman_code(replaced_text);
		message_bits = data_compression::compress(replaced_text, huffman_code);
		vector<pair<char, pair<uint32_t, int>>> codebook(huffman_code.begin(), huffman_code.end());
		sort(
		    codebook.begin(), codebook.end(),
		    [](const pair<char, pair<uint32_t, int>> &a, const pair<char, pair<uint32_t, int>> &b) {
			    if (a.second.second == b.second.second) {
				    if (a.second.first == b.second.first) {
					    return a.first < b.first;
				    }
				    return a.second.first < b.second.first;
			    }
			    return a.second.second < b.second.second;
		    });
		int max_bitlen = codebook.back().second.second;
		vector<char> codebook_chars;
		vector<int> codebook_char_num_per_bitlen(max_bitlen, 0);
		for (const auto &entry : codebook) {
			codebook_chars.push_back(entry.first);
			codebook_char_num_per_bitlen[entry.second.second - 1]++;
		}
		uint8_t num_chars = codebook_chars.size();
		for (int i = 7; i >= 0; i--) {
			codebook_bits.push_back((num_chars >> i) & 1);
		}
		for (int i = 7; i >= 0; i--) {
			codebook_bits.push_back((max_bitlen >> i) & 1);
		}
		uint8_t max_bits_to_encode_num_chars = int_log2(num_chars) + 1;
		for (int i = 1; i <= max_bitlen; i++) {
			for (int j = max_bits_to_encode_num_chars - 1; j >= 0; j--) {
				codebook_bits.push_back((codebook_char_num_per_bitlen[i - 1] >> j) & 1);
			}
		}
		for (const char c : codebook_chars) {
			for (int i = 6; i >= 0; i--) {
				codebook_bits.push_back((c >> i) & 1);
			}
		}
	}
	for (const bool bit : codebook_bits) {
		outstream->put(bit ? '1' : '0');
	}
	for (const bool bit : message_bits) {
		outstream->put(bit ? '1' : '0');
	}
}
