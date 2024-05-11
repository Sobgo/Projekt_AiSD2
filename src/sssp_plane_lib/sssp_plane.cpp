#include "sssp_plane.hpp"

/***
 * @brief Computes Single Source Shortest Path on a 2d plane
 * @param points: points on the plane
 * @param edges: edges described by indeces of points in `points`
 * @param source: index of the source point in `points`
 * @param destinations: indices of destination points in `points`
 * @return pairs of the index of the destination point and the path to it
 */
std::vector<std::pair<std::size_t, std::vector<std::size_t>>>
sssp_plane(const std::vector<std::pair<double, double>> &points,
           const std::vector<std::pair<std::size_t, std::size_t>> &edges, const std::size_t source,
           const std::vector<std::size_t> &destinations) {
	return {};
}