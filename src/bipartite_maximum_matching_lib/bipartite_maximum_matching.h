#ifndef BIPARTITE_MAXIMUM_MATCHING_H
#define BIPARTITE_MAXIMUM_MATCHING_H

#include <cstddef>
#include <utility>
#include <vector>

std::vector<std::pair<std::size_t, std::size_t>>
bipartite_maximum_matching(const std::vector<std::pair<std::size_t, std::size_t>> &pairs);

#endif