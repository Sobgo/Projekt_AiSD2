#ifndef DYNAMIC_OPTIMAL_PATH_HPP
#define DYNAMIC_OPTIMAL_PATH_HPP

#include <cstddef>
#include <utility>
#include <vector>

namespace dynamic_optimal_path {

std::pair<std::vector<std::size_t>, int> find_optimal_path(const std::vector<int> &brightness,
                                                           int maxStops);

}

#endif
