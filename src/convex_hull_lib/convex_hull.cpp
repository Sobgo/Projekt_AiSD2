#include "convex_hull.hpp"
#include <algorithm>
#include <cstddef>
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
 * @returns vector of input points indexes forming convex hull in counter-clockwise order
 * starting from the point with the lowest y coordinate (and the lowest x coordinate in case of a
 * tie). The set of points is minimal, so if there are multiple points on the same line, only the
 * endpoints are included.
 */
vector<size_t> convex_hull(const vector<Point> &points) {
	if (points.empty()) return {};
	if (points.size() == 1) return {0};

	vector<pair<Point, size_t>> indexed_points;
	for (size_t i = 0; i < points.size(); ++i) {
		indexed_points.emplace_back(points[i], i);
	}

	Point p0 = *min_element(points.begin(), points.end(), [](const Point &a, const Point &b) {
		return a.y == b.y ? a.x < b.x : a.y < b.y;
	});

	sort(indexed_points.begin(), indexed_points.end(),
	     [&p0](const pair<Point, size_t> &a, const pair<Point, size_t> &b) {
		     const double o = orientation(p0, a.first, b.first);

		     if (o == 0) {
			     const double dist_a = distance(p0, a.first);
			     const double dist_b = distance(p0, b.first);
			     return dist_a < dist_b;
		     }

		     return o < 0;
	     });

	vector<size_t> hull;

	for (const pair<Point, size_t> &p : indexed_points) {
		while (hull.size() >= 2) {
			const Point &p1 = points[hull[hull.size() - 2]];
			const Point &p2 = points[hull[hull.size() - 1]];

			if (orientation(p1, p2, p.first) < 0) break;
			hull.pop_back();
		}

		hull.push_back(p.second);
	}

	return hull;
}

#undef x
#undef y

}
