#include <cstddef>
#include <cstring>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

#include "../bipartite_maximum_matching_lib/bipartite_maximum_matching.hpp"

using namespace std;

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

	vector<pair<size_t, size_t>> input;
	size_t a = 0;
	size_t b = 0;
	while (*instream >> a >> b) {
		input.emplace_back(a - 1, b - 1);
	}
	auto result = bipartite_maximum_matching::bipartite_maximum_matching(input);
	for (auto &p : result) {
		*outstream << p.first + 1 << " " << p.second + 1 << '\n';
	}

	return 0;
}
