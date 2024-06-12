#ifndef SSSP_PLANE_H
#define SSSP_PLANE_H

#include <cstddef>
#include <utility>
#include <vector>

namespace sssp_plane {

using Point = std::pair<double, double>;
using Edge = std::pair<std::size_t, std::size_t>;

/**
 * @brief result type for sssp_plane::sssp_plane()
 */
struct SSSP_Path {
	/**
	 * @brief index of the destination vertex
	 */
	std::size_t destination;
	/**
	 * @brief indices of vertices forming the path
	 */
	std::vector<std::size_t> path;
	/**
	 * @brief length of the path
	 */
	double length;

	SSSP_Path(std::size_t destination, const std::vector<std::size_t> &path, double distance);
	SSSP_Path(std::size_t destination, std::vector<std::size_t> &&path, double distance);
	bool operator==(const SSSP_Path &other) const;
};

std::vector<SSSP_Path> sssp_plane(const std::vector<Point> &points, const std::vector<Edge> &edges,
                                  std::size_t source, const std::vector<std::size_t> &destinations);

}

#endif