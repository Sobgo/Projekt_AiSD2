#ifndef SSSP_PLANE_H
#define SSSP_PLANE_H

#include <cstddef>
#include <utility>
#include <vector>

namespace sssp_plane {

struct SSSP_Path {
	std::size_t destination;
	std::vector<std::size_t> path;
	double distance;

	SSSP_Path(std::size_t destination, std::vector<std::size_t> path, double distance);
	bool operator==(const SSSP_Path &other) const;
};

std::vector<SSSP_Path> sssp_plane(const std::vector<std::pair<double, double>> &points,
                                  const std::vector<std::pair<std::size_t, std::size_t>> &edges,
                                  std::size_t source, const std::vector<std::size_t> &destinations);

}

#endif