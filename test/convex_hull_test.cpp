#include "../src/convex_hull_lib/convex_hull.hpp"
#include <catch2/catch_test_macros.hpp>
#include <vector>
using namespace std;

#define x first
#define y second

TEST_CASE("Convex Hull Test 1") {
	vector<pair<double, double>> in = {{1, 1}, {2, 3}, {3, 2}, {2, 2}};
	vector<pair<double, double>> out = {{1, 1}, {2, 3}, {3, 2}};

	vector<pair<double, double>> hull = convex_hull(in);

	REQUIRE(hull.size() == out.size());

	for (int i = 0; i < hull.size(); ++i) {
		REQUIRE(hull[i].x == out[i].x);
		REQUIRE(hull[i].y == out[i].y);
	}
}

#undef x
#undef y
