#include "bipartite_maximum_matching.h"
#include <algorithm>
#include <cstdint>
#include <limits>
#include <optional>
#include <queue>

intmax_t bfs(size_t s, size_t t, std::vector<std::vector<intmax_t>> &G, std::vector<std::optional<size_t>> &parent) {
	std::queue<std::pair<int, int>> q;
	parent.assign(parent.size(), std::nullopt);
	q.push({s, std::numeric_limits<int>::max()});
	intmax_t new_flow = 0;
	while (!q.empty()) {
		size_t current = q.front().first;
		intmax_t current_flow = q.front().second;
		q.pop();
		for (size_t i = 0; i < G[current].size(); i++) {
			if (G[current][i] > 0 && !parent[i] && i != s) {
				parent[i] = current;
				new_flow = std::min(G[current][i], current_flow);
				q.push({i, new_flow});
			}
		}
	}
	if (!parent[t]) {
		return 0;
	}
	return new_flow;
}

std::pair<intmax_t, std::vector<std::vector<intmax_t>>> maxflow(std::vector<std::vector<intmax_t>> G, int s, int t) {
	std::vector<std::optional<size_t>> parent(G.size());
	auto flow = std::vector<std::vector<intmax_t>>(G.size(), std::vector<intmax_t>(G[0].size()));
	uintmax_t max_flow = 0;

	intmax_t new_flow = 0;
	while ((new_flow = bfs(s, t, G, parent)) > 0) {
		max_flow += new_flow;
		size_t current = t;
		while (current != s) {
			size_t p = *parent[current];
			G[p][current] -= new_flow;
			G[current][p] += new_flow;

			flow[p][current] += new_flow;
			flow[current][p] -= new_flow;

			current = p;
		}
	}

	return {max_flow, flow};
}

/***
 * @param pairs: Pairs of connected verticies. Indexing in each partition is separate.
 */
std::vector<std::pair<size_t, size_t>> bipartite_maximum_matching(const std::vector<std::pair<size_t, size_t>> &pairs) {
	if (pairs.empty()) {
		return {};
	}
	size_t max_left = 0;
	size_t max_right = 0;
	for (const auto &p : pairs) {
		max_left = std::max(max_left, p.first);
		max_right = std::max(max_right, p.second);
	}

	size_t left_num = max_left + 1;
	size_t right_num = max_right + 1;
	size_t vertices_num = left_num + right_num;

	size_t s = vertices_num;
	size_t t = s + 1;

	std::vector<std::vector<intmax_t>> G(vertices_num + 2, std::vector<intmax_t>(vertices_num + 2, 0));

	for (size_t i = 0; i < left_num; i++) {
		G[s][i] = 1;
	}
	for (size_t i = 0; i < right_num; i++) {
		G[left_num + i][t] = 1;
	}

	for (const auto &p : pairs) {
		G[p.first][left_num + p.second] = 1;
	}

	auto flow_result = maxflow(G, s, t);

	std::vector<std::pair<size_t, size_t>> result;

	for (size_t i = 0; i < left_num; i++) {
		for (size_t j = 0; j < right_num; j++) {
			if (flow_result.second[i][left_num + j] > 0) {
				result.emplace_back(i, j);
			}
		}
	}

	return result;
}
