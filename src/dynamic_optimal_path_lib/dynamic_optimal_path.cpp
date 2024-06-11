#include "dynamic_optimal_path.hpp"
#include <algorithm>
#include <climits>
#include <cstdint>

namespace find_optimal_path {

std::pair<std::vector<std::size_t>, int> optimalPath(const std::vector<std::size_t> &convexHullPoints,
                                                     const std::vector<int> &brightness, int maxStops) {

	std::vector<int> dp(convexHullPoints.size(), INT_MAX);

	std::vector<std::size_t> previous(convexHullPoints.size(), -1);

	dp[0] = 0;

	for (std::size_t i = 0; i < convexHullPoints.size(); ++i) {
		for (std::size_t j = 1; j <= maxStops && i + j < convexHullPoints.size(); ++j) {
			std::size_t next = i + j;
			if (brightness[convexHullPoints[i]] >
			    brightness[convexHullPoints[next]]) {
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
	for (std::size_t i = convexHullPoints.size() - 1; i != SIZE_MAX; i = previous[i]) {
		path.push_back(convexHullPoints[i]);
	}
	std::reverse(path.begin(), path.end());

	int minMelodyCount = dp[convexHullPoints.size() - 1];

	return {path, minMelodyCount};
}

} // namespace find_optimal_path