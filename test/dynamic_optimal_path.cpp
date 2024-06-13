#include "../src/dynamic_optimal_path_lib/dynamic_optimal_path.hpp"
#include <catch2/catch_test_macros.hpp>
#include <climits>
#include <functional>

std::pair<std::vector<std::vector<std::size_t>>, int> optimalBruteForce(const std::vector<std::size_t> &convexHullPoints,
                                                                        const std::vector<int> &brightness, int maxStops);

SCENARIO("Optimal path is found") {
	GIVEN("A set of points (short path)") {
		std::vector<std::size_t> convexHullPoints = {0, 1, 2, 3, 4, 5};
		std::vector<int> brightness = {10, 20, 30, 20, 10, 5};
		int maxStops = 2;

		auto result = find_optimal_path::optimalPath(convexHullPoints, brightness, maxStops);
		auto optimalPath = result.first;
		int optimalMelodyCount = result.second;

		auto resultBruteForce = optimalBruteForce(convexHullPoints, brightness, maxStops);
		auto bruteForcePaths = resultBruteForce.first;
		int bruteForceMelodyCount = resultBruteForce.second;

		REQUIRE(optimalMelodyCount == bruteForceMelodyCount);

		bool found = std::any_of(bruteForcePaths.begin(), bruteForcePaths.end(),
		                         [&optimalPath](const std::vector<std::size_t>& path) {
			                         return path == optimalPath;
		                         });

		REQUIRE(found);
	}

	GIVEN("A set of points (longer path)") {
		std::vector<std::size_t> convexHullPoints = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
		std::vector<int> brightness = {15, 10, 20, 25, 30, 18, 22, 28, 12, 16, 14};
		int maxStops = 3;

		auto result = find_optimal_path::optimalPath(convexHullPoints, brightness, maxStops);
		auto optimalPath = result.first;
		int optimalMelodyCount = result.second;

		auto resultBruteForce = optimalBruteForce(convexHullPoints, brightness, maxStops);
		auto bruteForcePaths = resultBruteForce.first;
		int bruteForceMelodyCount = resultBruteForce.second;

		REQUIRE(optimalMelodyCount == bruteForceMelodyCount);

		bool found = std::any_of(bruteForcePaths.begin(), bruteForcePaths.end(),
		                         [&optimalPath](const std::vector<std::size_t>& path) {
			                         return path == optimalPath;
		                         });

		REQUIRE(found);
	}

    GIVEN("Larger and more varied data set") {
		std::vector<std::size_t> convexHullPoints = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
		std::vector<int> brightness = {5, 3, 8, 2, 10, 1, 7, 9, 4, 6, 15, 12, 11, 14, 13, 20, 18, 17, 16, 19, 22};
		int maxStops = 5;

		auto result = find_optimal_path::optimalPath(convexHullPoints, brightness, maxStops);
		auto optimalPath = result.first;
		int optimalMelodyCount = result.second;

		auto resultBruteForce = optimalBruteForce(convexHullPoints, brightness, maxStops);
		auto bruteForcePaths = resultBruteForce.first;
		int bruteForceMelodyCount = resultBruteForce.second;

		REQUIRE(optimalMelodyCount == bruteForceMelodyCount);

		bool found = std::any_of(bruteForcePaths.begin(), bruteForcePaths.end(),
		                         [&optimalPath](const std::vector<std::size_t>& path) {
			                         return path == optimalPath;
		                         });

		REQUIRE(found);
	}
}

// ------- helper function implementation -------

std::pair<std::vector<std::vector<std::size_t>>, int> optimalBruteForce(const std::vector<std::size_t> &convexHullPoints,
                                                                        const std::vector<int> &brightness, int maxStops) {
	std::size_t n = convexHullPoints.size();
	int minMelodyCount = INT_MAX;
	std::vector<std::vector<std::size_t>> bestPaths;

	std::function<void(std::vector<std::size_t>&, int, std::size_t)> dfs = [&](std::vector<std::size_t>& path, int melodyCount, std::size_t currentIndex) {
		if (currentIndex == n - 1) {
			if (melodyCount < minMelodyCount) {
				minMelodyCount = melodyCount;
				bestPaths.clear();
				bestPaths.push_back(path);
			} else if (melodyCount == minMelodyCount) {
				bestPaths.push_back(path);
			}
			return;
		}

		for (std::size_t next = currentIndex + 1; next < std::min(currentIndex + maxStops + 1, n); ++next) {
			int newMelodyCount = melodyCount;
			if (brightness[convexHullPoints[currentIndex]] <= brightness[convexHullPoints[next]]) {
				newMelodyCount++;
			}
			path.push_back(convexHullPoints[next]);
			dfs(path, newMelodyCount, next);
			path.pop_back();
		}
	};

	std::vector<std::size_t> path = {convexHullPoints[0]};
	dfs(path, 0, 0);

	return {bestPaths, minMelodyCount};
}