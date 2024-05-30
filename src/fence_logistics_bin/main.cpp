#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "../convex_hull_lib/convex_hull.hpp"

using namespace std;

int main() {
	vector<pair<double, double>> points;
	string line;
	while (getline(cin, line) && !line.empty()) {
		istringstream iss(line);
		double x, y;
		iss >> x >> y;
		points.emplace_back(x, y);
	}
	auto convex_hull = convex_hull::convex_hull(points);
	double fence_length = 0;
	
	// TODO
}