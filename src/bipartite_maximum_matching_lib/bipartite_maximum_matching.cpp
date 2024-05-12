#include "bipartite_maximum_matching.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>
#include <queue>
#include <utility>
#include <vector>

namespace bipartite_maximum_matching {

int_fast8_t bfs(size_t start, std::vector<std::vector<int_fast8_t>> &graph,
                std::vector<std::optional<size_t>> &parent, size_t end) {
	std::queue<std::pair<size_t, int_fast8_t>> q; // NOLINT(readability-identifier-length)
	parent.assign(parent.size(), std::nullopt);
	q.emplace(start, std::numeric_limits<int_fast8_t>::max());
	int_fast8_t new_flow = 0;
	while (!q.empty()) {
		size_t current = q.front().first;
		const int_fast8_t current_flow = q.front().second;
		q.pop();
		for (size_t i = 0; i < graph[current].size(); i++) {
			if (graph[current][i] > 0 && !parent[i] && i != start) {
				parent[i] = current;
				new_flow = std::min(graph[current][i], current_flow);
				q.emplace(i, new_flow);
			}
		}
	}
	return parent[end] ? new_flow : 0;
}

std::vector<std::vector<int_fast8_t>>
maxflow(size_t source, std::vector<std::vector<int_fast8_t>> network, size_t sink) {
	std::vector<std::optional<size_t>> parent(network.size());
	auto flow = std::vector<std::vector<int_fast8_t>>(network.size(),
	                                                  std::vector<int_fast8_t>(network[0].size()));

	int_fast8_t new_flow = 0;
	while ((new_flow = bfs(source, network, parent, sink)) > 0) {
		size_t current = sink;
		while (current != source) {
			const size_t current_parent =
			    parent[current].value(); // NOLINT(bugprone-unchecked-optional-access)
			                             // zakładamy, że bfs działą poprawnie
			network[current_parent][current] -= new_flow;
			network[current][current_parent] += new_flow;

			flow[current_parent][current] += new_flow;
			flow[current][current_parent] -= new_flow;

			current = current_parent;
		}
	}

	return flow;
}

/***
 * @param pairs: Pairs of connected vertices. Indexing in each partition is separate.
 */
std::vector<std::pair<size_t, size_t>>
bipartite_maximum_matching(const std::vector<std::pair<size_t, size_t>> &pairs) {
	if (pairs.empty()) {
		return {};
	}
	size_t max_left = 0;
	size_t max_right = 0;
	for (const auto &pair : pairs) {
		max_left = std::max(max_left, pair.first);
		max_right = std::max(max_right, pair.second);
	}

	const size_t left_num = max_left + 1;
	const size_t right_num = max_right + 1;
	const size_t vertices_num = left_num + right_num;

	const size_t source = vertices_num;
	const size_t sink = source + 1;

	std::vector<std::vector<int_fast8_t>> network(vertices_num + 2,
	                                              std::vector<int_fast8_t>(vertices_num + 2, 0));

	for (size_t i = 0; i < left_num; i++) {
		network[source][i] = 1;
	}
	for (size_t i = 0; i < right_num; i++) {
		network[left_num + i][sink] = 1;
	}

	for (const auto &pair : pairs) {
		network[pair.first][left_num + pair.second] = 1;
	}

	auto flow_result = maxflow(source, network, sink);

	std::vector<std::pair<size_t, size_t>> result;

	for (size_t i = 0; i < left_num; i++) {
		for (size_t j = 0; j < right_num; j++) {
			if (flow_result[i][left_num + j] > 0) {
				result.emplace_back(i, j);
			}
		}
	}

	return result;
}

}
