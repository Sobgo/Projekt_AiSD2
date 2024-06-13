#include <cstddef>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "../dynamic_optimal_path_lib/dynamic_optimal_path.hpp"

using namespace std;

void process_guard(const vector<int> &brightness_values, int max_skips, ostream &outstream,
                   bool verbose_option);

int main(int argc, char *argv[]) {
	istream *instream = nullptr;
	ostream *outstream = nullptr;

	ifstream infile;
	ofstream outfile;

	// NOLINTBEGIN(cppcoreguidelines-pro-bounds-pointer-arithmetic)
	if (argc < 3) {
		cerr << "Usage: " << argv[0] << " <input file> <output file> [-v]\n";
		return 1;
	}

	const bool verbose_option = argc >= 4 && strcmp(argv[3], "-v") == 0;

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

	int temp = 0;
	vector<int> brightness_values;

	string line;
	getline(*instream, line);
	istringstream iss(line);

	while (iss >> temp) {
		if (temp < 0) {
			cerr << "Error: brightness values must be non-negative\n";
			return 1;
		}

		brightness_values.emplace_back(temp);
	}

	bool first = true;

	// read line and from each line one number
	while (getline(*instream, line)) {
		iss.clear();
		iss.str(line);

		if (!(iss >> temp)) {
			cerr << "Error: invalid input\n";
			return 1;
		}

		if (temp < 0) {
			cerr << "Error: Maximum skips value must be non-negative\n";
			return 1;
		}

		if (verbose_option) {
			if (!first) {
				*outstream << "\n";
			} else {
				first = false;
			}
		}

		process_guard(brightness_values, temp, *outstream, verbose_option);
	}
}

void process_guard(const vector<int> &brightness_values, int max_skips, ostream &outstream,
                   bool verbose_option) {
	auto result = dynamic_optimal_path::find_optimal_path(brightness_values, max_skips);
	vector<size_t> path = result.first;
	vector<size_t> rest_points;

	if (verbose_option) {
		if (max_skips == 1) {
			outstream << "With 1 maximum skip guard takes path: ";
		} else {
			outstream << "With " << max_skips << " maximum skips guard takes path: ";
		}
	} else {
		outstream << max_skips << " ";
	}

	for (size_t i = 0; i < path.size(); ++i) {
		outstream << path[i] + 1 << " ";

		if (i < path.size() - 1) {
			if (brightness_values[path[i]] < brightness_values[path[i + 1]]) {
				rest_points.push_back(path[i + 1] + 1);
			}
		}
	}

	if (verbose_option) {
		if (rest_points.size() == 1) {
			outstream << "\nAnd needs to rest 1 time at point: ";
		} else {
			outstream << "\nAnd needs to rest " << rest_points.size() << " times at points: ";
		}
	} else {
		outstream << "\n" << rest_points.size() << " ";
	}

	for (size_t i = 0; i < rest_points.size(); ++i) {
		outstream << rest_points[i];

		if (i < rest_points.size() - 1) {
			outstream << " ";
		}
	}

	outstream << "\n";
}
