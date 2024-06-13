#include "sssp_plane.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <functional>
#include <limits>
#include <optional>
#include <queue>
#include <stdexcept>
#include <utility>
#include <vector>

/**
 * @brief single source shortest path on a 2d plane
 */
namespace sssp_plane {

double distance(const std::pair<double, double> &a, const std::pair<double, double> &b) {
	const double x_dist = a.first - b.first;
	const double y_dist = a.second - b.second;
	return std::sqrt(x_dist * x_dist + y_dist * y_dist);
}

using AdjEdge = std::pair<std::size_t, double>;

void dijkstra(const std::vector<std::vector<AdjEdge>> &adj_list, const std::size_t source,
              std::vector<double> &dist, std::vector<std::optional<std::size_t>> &parent) {
	std::priority_queue<AdjEdge, std::vector<AdjEdge>, std::greater<>> q;
	dist[source] = 0;
	q.emplace(source, 0);
	while (!q.empty()) {
		const std::size_t current_vertex = q.top().first;
		const double current_dist = q.top().second;
		q.pop();

		if (dist[current_vertex] < current_dist) continue;

		for (const auto &edge : adj_list[current_vertex]) {
			const std::size_t v = edge.first;
			const double d = edge.second;
			const double potential_dist = current_dist + d;
			if (dist[v] > potential_dist) {
				dist[v] = potential_dist;
				parent[v] = current_vertex;
				q.emplace(v, dist[v]);
			}
		}
	}
}

SSSP_Path::SSSP_Path(std::size_t destination, const std::vector<std::size_t> &path, double distance)
    : destination(destination), path(path), length(distance) {}

SSSP_Path::SSSP_Path(std::size_t destination, std::vector<std::size_t> &&path, double distance)
    : destination(destination), path(std::move(path)), length(distance) {}

bool SSSP_Path::operator==(const SSSP_Path &other) const {
	return destination == other.destination && path == other.path && length == other.length;
}

/**
 * @brief Computes single source shortest path on a 2d plane
 * @details Uses Dijkstra's algorithm
 * @param points: points on the plane
 * @param edges: edges described by indeces of points in `points`, each edge must be defined once
 * (each direction is considered a separate edge)
 * @param source: index of the source point in `points`
 * @param destinations: indices of destination points in `points`
 * @return pairs of the index of the destination point and the path to it
 */
std::vector<SSSP_Path> sssp_plane(const std::vector<Point> &points, const std::vector<Edge> &edges,
                                  std::size_t source,
                                  const std::vector<std::size_t> &destinations) {
	if (source >= points.size()) {
		throw std::out_of_range("source index out of range");
	}
	for (const auto &edge : edges) {
		if (edge.first >= points.size() || edge.second >= points.size()) {
			throw std::out_of_range("edge index out of range");
		}
	}
	std::vector<std::vector<std::pair<std::size_t, double>>> adj_list(points.size());
	for (const auto &edge : edges) {
		const double dist = distance(points[edge.first], points[edge.second]);
		adj_list[edge.first].emplace_back(edge.second, dist);
	}

	std::vector<double> dist(points.size(), std::numeric_limits<double>::infinity());
	std::vector<std::optional<std::size_t>> parent(points.size(), std::nullopt);

	dijkstra(adj_list, source, dist, parent);

	std::vector<SSSP_Path> result;
	result.reserve(destinations.size());
	for (const auto &destination : destinations) {
		if (dist[destination] == std::numeric_limits<double>::infinity()) {
			continue;
		}
		std::vector<std::size_t> path;

		// this is checked by loop condition
		// NOLINTBEGIN(bugprone-unchecked-optional-access)
		for (std::optional<std::size_t> current = destination; current.has_value();
		     current = parent[current.value()]) {
			path.push_back(current.value());
		}
		// NOLINTEND(bugprone-unchecked-optional-access)
		std::reverse(path.begin(), path.end());
		result.emplace_back(destination, path, dist[destination]);
	}

	return result;
}

}
