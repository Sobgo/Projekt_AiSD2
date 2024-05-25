#include "convex_hull.hpp"
#include <algorithm>
#include <utility>
#include <vector>

namespace convex_hull {

using namespace std;
using Point = pair<double, double>;

#define x first  // NOLINT(cppcoreguidelines-macro-usage)
#define y second // NOLINT(cppcoreguidelines-macro-usage)

double distance(const Point &a, const Point &b) {
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

double orientation(const Point &a, const Point &b, const Point &c) {
	return (a.x - b.x) * (c.y - b.y) - (a.y - b.y) * (c.x - b.x);
}

/**
 * O(nlogn)
 * @param points vector of pairs where first is x coordinate and second is y coordinate of a point
 * @returns the convex hull in counter-clockwise order starting from the point with the lowest y
 * coordinate (and the lowest x coordinate in case of a tie).
 * The set of points is minimal, so if there are multiple points on the same line, only the
 * endpoints are included.
 */
vector<Point> convex_hull(vector<Point> points) {
	if (points.size() <= 1) return points;

	Point p0 = *min_element(points.begin(), points.end(), [](const Point &a, const Point &b) {
		return a.y == b.y ? a.x < b.x : a.y < b.y;
	});

	sort(points.begin(), points.end(), [&p0](const Point &a, const Point &b) {
		const double o = orientation(p0, a, b);

		if (o == 0) {
			const double dist_a = distance(p0, a);
			const double dist_b = distance(p0, b);
			return dist_a < dist_b;
		}

		return o < 0;
	});

	vector<Point> hull;

	for (const Point &p : points) {
		while (hull.size() >= 2) {
			const Point &p1 = hull[hull.size() - 2];
			const Point &p2 = hull[hull.size() - 1];

			if (orientation(p1, p2, p) < 0) break;
			hull.pop_back();
		}

		hull.push_back(p);
	}

	return hull;
}

#undef x
#undef y

}
