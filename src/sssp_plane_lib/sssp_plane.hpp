#ifndef SSSP_PLANE_H
#define SSSP_PLANE_H

#include <cstddef>
#include <utility>
#include <vector>

namespace sssp_plane {

typedef std::pair<double, double> Point;
typedef std::pair<std::size_t, std::size_t> Edge;

struct SSSP_Path {
	std::size_t destination;
	std::vector<std::size_t> path;
	double distance;

	SSSP_Path(std::size_t destination, std::vector<std::size_t> path, double distance);
	bool operator==(const SSSP_Path &other) const;
};

std::vector<SSSP_Path> sssp_plane(const std::vector<Point> &points, const std::vector<Edge> &edges,
                                  std::size_t source, const std::vector<std::size_t> &destinations);

}

#endif