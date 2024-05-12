#include "sssp_plane.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <limits>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

namespace sssp_plane {

double distance(const std::pair<double, double> &a, const std::pair<double, double> &b) {
	const double x_dist = a.first - b.first;
	const double y_dist = a.second - b.second;
	return std::sqrt(x_dist * x_dist + y_dist * y_dist);
}

/***
 * @brief Computes Single Source Shortest Path on a 2d plane
 * @details Uses Dijkstra's algorithm
 * @param points: points on the plane
 * @param edges: edges described by indeces of points in `points`, each edge must be defined once
 * (each direction is considered a separate edge)
 * @param source: index of the source point in `points`
 * @param destinations: indices of destination points in `points`
 * @return pairs of the index of the destination point and the path to it
 */
std::vector<std::pair<std::size_t, std::vector<std::size_t>>>
sssp_plane(const std::vector<std::pair<double, double>> &points,
           const std::vector<std::pair<std::size_t, std::size_t>> &edges, const std::size_t source,
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
		adj_list[edge.first].push_back({edge.second, dist});
	}

	std::vector<double> dist(points.size(), std::numeric_limits<double>::infinity());
	std::vector<std::optional<std::size_t>> parent(points.size(), std::nullopt);
	std::vector<bool> visited(points.size(), false);
	dist[source] = 0;
	for (std::size_t i = 0; i < points.size(); ++i) {
		std::size_t current_closest_vertex = 0;
		{
			std::optional<std::size_t> current_closest_vertex_opt = std::nullopt;
			for (std::size_t j = 0; j < points.size(); ++j) {
				if (!visited[j] &&
				    (!current_closest_vertex_opt || dist[j] < dist[*current_closest_vertex_opt])) {
					current_closest_vertex_opt = j;
				}
			}
			current_closest_vertex = *current_closest_vertex_opt;
		}
		if (dist[current_closest_vertex] == std::numeric_limits<double>::infinity()) {
			break;
		}
		visited[current_closest_vertex] = true;
		for (const auto &edge : adj_list[current_closest_vertex]) {
			const double new_dist = dist[current_closest_vertex] + edge.second;
			if (new_dist < dist[edge.first]) {
				dist[edge.first] = new_dist;
				parent[edge.first] = current_closest_vertex;
			}
		}
	}

	std::vector<std::pair<std::size_t, std::vector<std::size_t>>> result;
	result.reserve(destinations.size());
	for (const auto &destination : destinations) {
		if (dist[destination] == std::numeric_limits<double>::infinity()) {
			continue;
		}
		std::vector<std::size_t> path;
		for (std::size_t current = destination; current != source; current = *parent[current]) {
			path.push_back(current);
		}
		std::reverse(path.begin(), path.end());
		result.push_back({destination, path});
	}

	return result;
}

} // namespace sssp_plane
