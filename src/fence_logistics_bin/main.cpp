#include <cmath>
#include <cstddef>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "../convex_hull_lib/convex_hull.hpp"
#include "../sssp_plane_lib/sssp_plane.hpp"

using namespace std;

double euclidean_distance(const pair<double, double> &a, const pair<double, double> &b) {
	const double dx = a.first - b.first;
	const double dy = a.second - b.second;
	return sqrt(dx * dx + dy * dy);
}

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

	vector<pair<double, double>> points;
	size_t factory_idx = 0;

	string line;
	while (getline(*instream, line)) {
		istringstream iss(line);
		double x = 0;
		double y = 0;
		iss >> x;
		if (iss >> y) {
			points.emplace_back(x, y);
		} else {
			factory_idx = (size_t)x;
			break;
		}
	}
	if (points.empty()) {
		cerr << "Error: no points entered\n";
		return 1;
	}
	if (factory_idx < 1) {
		cerr << "Error: factory index has to be entered and greater than 0\n";
		return 1;
	}
	factory_idx--;
	if (factory_idx >= points.size()) {
		cerr << "Error: factory index out of bounds\n";
		return 1;
	}

	auto convex_hull = convex_hull::convex_hull(points);
	double fence_length = 0;
	for (int i = 0; i < convex_hull.size() - 1; i++) {
		fence_length += euclidean_distance(points[convex_hull[i]], points[convex_hull[i + 1]]);
	}
	fence_length += euclidean_distance(points[convex_hull.back()], points[convex_hull.front()]);

	vector<pair<size_t, size_t>> edges;
	size_t a = 0;
	size_t b = 0;
	while (*instream >> a >> b) {
		edges.emplace_back(a - 1, b - 1);
	}

	auto routes = sssp_plane::sssp_plane(points, edges, factory_idx, convex_hull);

	if (verbose_option) {
		*outstream << "Fence length: " << fence_length << '\n';
		*outstream << "Fence:";
		for (const auto &point : convex_hull) {
			*outstream << ' ' << point + 1;
		}
		*outstream << '\n';
		*outstream << "Routes:\n";
	} else {
		*outstream << fence_length << '\n';
		if (!convex_hull.empty()) {
			*outstream << convex_hull.front() + 1;
		}
		for (size_t i = 1; i < convex_hull.size(); i++) {
			*outstream << ' ' << convex_hull[i] + 1;
		}
		*outstream << '\n';
	}
	for (const auto &route : routes) {
		if (verbose_option) {
			*outstream << "destination: " << route.destination + 1 << ", length: " << route.distance
			           << ", path:";
		} else {
			*outstream << route.destination + 1 << ' ' << route.distance;
		}
		for (const auto &point : route.path) {
			*outstream << ' ' << point + 1;
		}
		*outstream << '\n';
	}
}
