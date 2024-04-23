#include "bipartite_maximum_matching.h"
#include <cstdint>
#include <limits>
#include <optional>
#include <queue>

int_fast8_t bfs(size_t s, size_t t, std::vector<std::vector<int_fast8_t>> &G, std::vector<std::optional<size_t>> &parent) {
	std::queue<std::pair<size_t, size_t>> q;
	parent.assign(parent.size(), std::nullopt);
	q.push({s, std::numeric_limits<int_fast8_t>::max()});
	int_fast8_t new_flow = 0;
	while (!q.empty()) {
		size_t current = q.front().first;
		int_fast8_t current_flow = q.front().second;
		q.pop();
		for (size_t i = 0; i < G[current].size(); i++) {
			if (G[current][i] > 0 && !parent[i] && i != s) {
				parent[i] = current;
				new_flow = std::min(G[current][i], current_flow);
				q.push({i, new_flow});
			}
		}
	}
	return parent[t] ? new_flow : 0;
}

std::vector<std::vector<int_fast8_t>> maxflow(std::vector<std::vector<int_fast8_t>> G, size_t s, size_t t) {
	std::vector<std::optional<size_t>> parent(G.size());
	auto flow = std::vector<std::vector<int_fast8_t>>(G.size(), std::vector<int_fast8_t>(G[0].size()));

	int_fast8_t new_flow = 0;
	while ((new_flow = bfs(s, t, G, parent)) > 0) {
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

	return flow;
}

/***
 * @param pairs: Pairs of connected vertices. Indexing in each partition is separate.
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

	std::vector<std::vector<int_fast8_t>> G(vertices_num + 2, std::vector<int_fast8_t>(vertices_num + 2, 0));

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
			if (flow_result[i][left_num + j] > 0) {
				result.emplace_back(i, j);
			}
		}
	}

	return result;
}
