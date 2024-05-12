#ifndef SSSP_PLANE_H
#define SSSP_PLANE_H

#include <utility>
#include <vector>

namespace sssp_plane {

std::vector<std::pair<std::size_t, std::vector<std::size_t>>>
sssp_plane(const std::vector<std::pair<double, double>> &points,
           const std::vector<std::pair<std::size_t, std::size_t>> &edges, const std::size_t source,
           const std::vector<std::size_t> &destinations);

}

#endif