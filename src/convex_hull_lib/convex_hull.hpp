#ifndef CONVEX_HULL_HPP
#define CONVEX_HULL_HPP

#include <cstddef>
#include <utility>
#include <vector>

namespace convex_hull {

using Point = std::pair<double, double>;

double distance(const Point &a, const Point &b);

double orientation(const Point &a, const Point &b, const Point &c);

std::vector<std::size_t> convex_hull(const std::vector<Point> &points);

}

#endif
