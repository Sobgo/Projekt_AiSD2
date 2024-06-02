#include <cmath>
#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "../convex_hull_lib/convex_hull.hpp"
#include "../sssp_plane_lib/sssp_plane.hpp"

using namespace std;

double euclidean_distance(const pair<double, double> &a, const pair<double, double> &b) {
	double dx = a.first - b.first;
	double dy = a.second - b.second;
	return sqrt(dx * dx + dy * dy);
}

int main() {
	vector<pair<double, double>> points;
	string line;
	while (getline(cin, line) && !line.empty()) {
		istringstream iss(line);
		double x = 0;
		double y = 0;
		iss >> x >> y;
		points.emplace_back(x, y);
	}
	auto convex_hull = convex_hull::convex_hull(points);
	double fence_length = 0;

	for (int i = 0; i < convex_hull.size() - 1; i++) {
		fence_length += euclidean_distance(points[convex_hull[i]], points[convex_hull[i + 1]]);
	}
	fence_length += euclidean_distance(points[convex_hull.back()], points[convex_hull.front()]);

	cout << fence_length << '\n';

	size_t factory_idx = 0;
	cin >> factory_idx;
	factory_idx--;

	vector<pair<size_t, size_t>> edges;
	size_t a = 0;
	size_t b = 0;
	while (cin >> a >> b) {
		edges.emplace_back(a - 1, b - 1);
	}

	auto routes = sssp_plane::sssp_plane(points, edges, factory_idx, convex_hull);

	for (const auto &route : routes) {
		cout << '\n' << route.destination + 1 << ' ' << route.distance;
		for (const auto &point : route.path) {
			cout << ' ' << point + 1;
		}
	}
	cout << '\n';
}
