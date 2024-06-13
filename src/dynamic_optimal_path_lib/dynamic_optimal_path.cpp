#include "dynamic_optimal_path.hpp"
#include <algorithm>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>

/**
 * @brief dynamic_optimal_path
 */
namespace dynamic_optimal_path {

/**
 * @brief Finds path with minimal number of stops required.
 * @details Finds path with minimal number of stops required and number of resting spots. Path is
 * returned as vector of indices of points from input vector where guard needs to stop. Exact
 * resting stops can be aquired by simple linear search where previous stop from path has more
 * brightness than next stop.
 * @param brightness vector of brightness values at each point
 * @param maxBeforeStop maximum number of points thar can be skipped before stop is required
 * @return vector of indices of optimal path and minimal number of required stops as pair
 */
std::pair<std::vector<std::size_t>, int> find_optimal_path(const std::vector<int> &brightness,
                                                           int maxBeforeStop) {

	std::vector<int> dp(brightness.size(), INT_MAX);

	std::vector<std::size_t> previous(brightness.size(), -1);

	dp[0] = 0;

	for (std::size_t i = 0; i < brightness.size(); ++i) {
		for (std::size_t j = 1; j <= maxBeforeStop && i + j < brightness.size(); ++j) {
			const std::size_t next = i + j;
			if (brightness[i] > brightness[next]) {
				if (dp[i] < dp[next]) {
					dp[next] = dp[i];
					previous[next] = i;
				}
			} else {
				if (dp[i] + 1 < dp[next]) {
					dp[next] = dp[i] + 1;
					previous[next] = i;
				}
			}
		}
	}

	std::vector<std::size_t> path;
	for (std::size_t i = brightness.size() - 1; i != SIZE_MAX; i = previous[i]) {
		path.push_back(i);
	}
	std::reverse(path.begin(), path.end());

	const int minMelodyCount = dp[brightness.size() - 1];

	return {path, minMelodyCount};
}

}
