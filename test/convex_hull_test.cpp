#include "../src/convex_hull_lib/convex_hull.hpp"
#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <vector>
using namespace std;

using Point = pair<double, double>;

bool is_subset(vector<Point> a, vector<Point> b);
bool is_convex(vector<Point> &polygon);
bool all_inside_polygon(vector<Point> &points, vector<Point> &polygon);

SCENARIO("Convex Hull is calculated correctly") {
	GIVEN("A set of points") {
		vector<Point> in = {{1, 1}, {2, 3}, {3, 2}, {2, 2}};

		WHEN("Convex Hull is calculated") {
			vector<Point> hull = convex_hull(in);

			THEN("It is a subset of the input") {
				REQUIRE(is_subset(hull, in));
			}

			THEN("It has at least 3 points") { REQUIRE(hull.size() >= 3); }

			THEN("It is convex") { REQUIRE(is_convex(hull)); }

			THEN("All input points are inside the hull") {
				REQUIRE(all_inside_polygon(in, hull));
			}
		}
	}

	// TODO edge cases

	// TODO randomized tests
}

// ------- helper functions implementation -------

#define x first
#define y second

// O(nlogn)
bool is_subset(vector<Point> a, vector<Point> b) {
	sort(a.begin(), a.end());
	sort(b.begin(), b.end());

	return includes(a.begin(), a.end(), b.begin(), b.end());
}

// O(n)
bool is_convex(vector<Point> &polygon) {
	bool has_positive = false;
	bool has_negative = false;

	for (int i = 0; i < polygon.size(); ++i) {
		Point &a = polygon[i];
		Point &b = polygon[(i + 1) % polygon.size()];
		Point &c = polygon[(i + 2) % polygon.size()];

		double cross_product =
			(b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x);

		if (cross_product > 0)
			has_positive = true;
		else if (cross_product < 0)
			has_negative = true;

		if (has_positive && has_negative) return false;
	}

	return true;
}

// O(n^2) - this can be optimized to O(nlogn) using more sophisticated algorithm
bool all_inside_polygon(vector<Point> &points, vector<Point> &polygon) {
	// points on the boundary are considered inside

	for (Point &p : points) {
		bool is_inside = false;
		for (int i = 0; i < polygon.size(); ++i) {
			Point &a = polygon[i];
			Point &b = polygon[(i + 1) % polygon.size()];

			if ((b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x) >= 0) {
				is_inside = true;
				break;
			}
		}

		if (!is_inside) return false;
	}

	return true;
}

#undef x
#undef y
