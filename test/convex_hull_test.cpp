#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <ctime>
#include <random>
#include <set>
#include <utility>
#include <vector>

#include "../src/convex_hull_lib/convex_hull.hpp"

using namespace std;
using Point = pair<double, double>;

bool is_subset(vector<Point> a, vector<Point> b);
bool is_convex(const vector<Point> &polygon);
bool all_inside_polygon(const vector<Point> &points, const vector<Point> &polygon);

SCENARIO("Convex Hull is calculated correctly") {
	GIVEN("A set of points") {
		vector<Point> in = {{1, 1}, {2, 3}, {3, 2}, {2, 2}};
		vector<Point> out = {{1, 1}, {2, 3}, {3, 2}};

		WHEN("Convex Hull is calculated") {
			vector<size_t> hull = convex_hull::convex_hull(in);
			vector<Point> hull_points;
			for (size_t i : hull) {
				hull_points.push_back(in[i]);
			}

			THEN("It matches the expected output") {
				REQUIRE(hull_points.size() == out.size());
				REQUIRE(is_subset(hull_points, out));
			}
		}
	}

	GIVEN("Empty set") {
		vector<Point> in;

		WHEN("Convex Hull is calculated") {
			vector<size_t> hull = convex_hull::convex_hull(in);

			THEN("It is empty") { REQUIRE(hull.empty()); }
		}
	}

	GIVEN("A set with one point") {
		vector<Point> in = {{7.3, 8.23}};

		WHEN("Convex Hull is calculated") {
			vector<size_t> hull = convex_hull::convex_hull(in);
			vector<Point> hull_points;
			for (size_t i : hull) {
				hull_points.push_back(in[i]);
			}

			THEN("It is the same point") { REQUIRE(hull_points == in); }
		}
	}

	GIVEN("A set of 2 points") {
		vector<Point> in = {{3.14, 2.71}, {1.61, 9.81}};

		WHEN("Convex Hull is calculated") {
			vector<size_t> hull = convex_hull::convex_hull(in);
			vector<Point> hull_points;
			for (size_t i : hull) {
				hull_points.push_back(in[i]);
			}

			THEN("It is the same set") {
				REQUIRE(hull_points.size() == in.size());
				REQUIRE(is_subset(hull_points, in));
			}
		}
	}

	GIVEN("A set of 3 collinear points") {
		vector<Point> in = {{1, 1}, {2, 2}, {3, 3}};

		WHEN("Convex Hull is calculated") {
			vector<size_t> hull = convex_hull::convex_hull(in);
			vector<Point> hull_points;
			for (size_t i : hull) {
				hull_points.push_back(in[i]);
			}

			THEN("It contains only the first and last points") {
				REQUIRE(hull_points.size() == 2);
				REQUIRE(is_subset(hull_points, in));
				REQUIRE(find(hull_points.begin(), hull_points.end(), in[1]) == hull_points.end());
			}
		}
	}

	GIVEN("A set of 3 points on vertical line") {
		vector<Point> in = {{1, 1}, {1, 2}, {1, 3}};

		WHEN("Convex Hull is calculated") {
			vector<size_t> hull = convex_hull::convex_hull(in);
			vector<Point> hull_points;

			for (size_t i : hull) {
				hull_points.push_back(in[i]);
			}

			THEN("The hull contains only the first and last points") {
				REQUIRE(hull_points.size() == 2);
				REQUIRE(is_subset(hull_points, in));
				REQUIRE(find(hull_points.begin(), hull_points.end(), in[1]) == hull_points.end());
			}
		}
	}

	GIVEN("A set of 3 points on horizontal line") {
		vector<Point> in = {{1, 1}, {2, 1}, {3, 1}};

		WHEN("Convex Hull is calculated") {
			vector<size_t> hull = convex_hull::convex_hull(in);
			vector<Point> hull_points;

			for (size_t i : hull) {
				hull_points.push_back(in[i]);
			}

			THEN("The hull contains only the first and last points") {
				REQUIRE(hull_points.size() == 2);
				REQUIRE(is_subset(hull_points, in));
				REQUIRE(find(hull_points.begin(), hull_points.end(), in[1]) == hull_points.end());
			}
		}
	}

	GIVEN("10 randomized tests with sets of 100 points") {
		// the given/when structure here not matching
		// previous tests is due to how Catch2 works with loops
		// (ie. doesn't work)

		WHEN("Convex Hull is calculated") {
			for (int i = 0; i < 10; ++i) {
				uniform_real_distribution<double> dist(-1000, 1000);
				default_random_engine gen(time(NULL));

				set<Point> in;
				while (in.size() < 100) {
					in.insert({dist(gen), dist(gen)});
				}

				vector<Point> in_vec(in.begin(), in.end());
				vector<size_t> hull = convex_hull::convex_hull(in_vec);
				vector<Point> hull_points;

				for (size_t i : hull) {
					hull_points.push_back(in_vec[i]);
				}

				THEN("It is a subset of the input") { REQUIRE(is_subset(hull_points, in_vec)); }

				THEN("It is convex") { REQUIRE(is_convex(hull_points)); }

				THEN("All input points are inside the hull") {
					REQUIRE(all_inside_polygon(in_vec, hull_points));
				}
			}
		}
	}
}

// ------- helper functions implementation -------

#define x first
#define y second

// O(nlogn)
// checks if A is a subset of B
bool is_subset(vector<Point> A, vector<Point> B) {
	sort(A.begin(), A.end());
	sort(B.begin(), B.end());

	return includes(B.begin(), B.end(), A.begin(), A.end());
}

// O(n)
bool is_convex(const vector<Point> &polygon) {
	bool has_positive = false;
	bool has_negative = false;

	for (int i = 0; i < polygon.size(); ++i) {
		const Point &a = polygon[i];
		const Point &b = polygon[(i + 1) % polygon.size()];
		const Point &c = polygon[(i + 2) % polygon.size()];

		double o = convex_hull::orientation(a, b, c);

		if (o > 0) {
			has_positive = true;
		} else if (o < 0) {
			has_negative = true;
		}

		if (has_positive && has_negative) return false;
	}

	return true;
}

// O(n^2) - this can be optimized to O(nlogn) using more sophisticated algorithm
// points on the boundary are considered inside
bool all_inside_polygon(const vector<Point> &points, const vector<Point> &polygon) {
	const auto between = [](double p, double a, double b) -> bool {
		return p >= a && p <= b || p <= a && p >= b;
	};

	for (const Point &p : points) {
		bool is_inside = false;

		for (int i = polygon.size() - 1, j = 0; j < polygon.size(); i = j++) {
			const Point &a = polygon[i];
			const Point &b = polygon[j];

			if (p.x == a.x && p.y == a.y || p.x == b.x && p.y == b.y) {
				is_inside = true;
				break;
			}

			if (a.y == b.y && p.y == a.y && between(p.x, a.x, b.x)) {
				is_inside = true;
				break;
			}

			if (between(p.y, a.y, b.y)) {
				if ((p.y == a.y && b.y >= a.y) || (p.y == b.y && a.y >= b.y)) {
					is_inside = true;
					break;
				}

				const double o = convex_hull::orientation(a, b, p);
				if (o == 0) continue;

				if ((a.y < b.y) == (o > 0)) {
					is_inside = !is_inside;
				}
			}
		}

		if (!is_inside) return false;
	}

	return true;
}

#undef x
#undef y
