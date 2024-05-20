#ifndef CONVEX_HULL_HPP
#define CONVEX_HULL_HPP

#include <utility>
#include <vector>

namespace convex_hull {

double distance(const std::pair<double, double> &a, const std::pair<double, double> &b);

double orientation(const std::pair<double, double> &a, const std::pair<double, double> &b,
                   const std::pair<double, double> &c);

std::vector<std::pair<double, double>> convex_hull(std::vector<std::pair<double, double>> points);

}

#endif
