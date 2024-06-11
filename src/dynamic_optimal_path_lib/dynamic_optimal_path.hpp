#ifndef DYNAMIC_OPTIMAL_PATH_HPP
#define DYNAMIC_OPTIMAL_PATH_HPP

#include <vector>

namespace find_optimal_path {

std::pair<std::vector<std::size_t>, int> optimalPath(const std::vector<std::size_t> &convexHullPoints,
                                                 const std::vector<int> &brightness, int maxStops);

} // namespace find_optimal_path

#endif
