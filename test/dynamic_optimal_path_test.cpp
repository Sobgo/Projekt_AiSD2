#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <climits>
#include <cstddef>
#include <functional>

#include "../src/dynamic_optimal_path_lib/dynamic_optimal_path.hpp"

std::pair<std::vector<std::vector<std::size_t>>, int>
optimal_path_brute_force(const std::vector<int> &brightness, int maxStops);

SCENARIO("Optimal path is found") {
	GIVEN("A set of points (short path)") {
		std::vector<int> brightness = {10, 20, 30, 20, 10, 5};
		int maxStops = 2;

		auto result = dynamic_optimal_path::find_optimal_path(brightness, maxStops);
		std::vector<std::size_t> expectedPath = {0, 2, 3, 5};
		int expectedMinMelodyCount = 1;

		REQUIRE(result.first == expectedPath);
		REQUIRE(result.second == expectedMinMelodyCount);
	}

	GIVEN("Larger and more varied data set") {
		std::vector<int> brightness = {5,  3,  8,  2,  10, 1,  7,  9,  4,  6, 15,
		                               12, 11, 14, 13, 20, 18, 17, 16, 19, 22};
		int maxStops = 5;

		auto result = dynamic_optimal_path::find_optimal_path(brightness, maxStops);
		auto optimalPath = result.first;
		int optimalMelodyCount = result.second;

		auto resultBruteForce = optimal_path_brute_force(brightness, maxStops);
		auto bruteForcePaths = resultBruteForce.first;
		int bruteForceMelodyCount = resultBruteForce.second;

		REQUIRE(optimalMelodyCount == bruteForceMelodyCount);

		bool found = std::any_of(
		    bruteForcePaths.begin(), bruteForcePaths.end(),
		    [&optimalPath](const std::vector<std::size_t> &path) { return path == optimalPath; });

		REQUIRE(found);
	}

	GIVEN("All points with the same brightness") {
		std::vector<int> brightness = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10};
		int maxStops = 3;

		auto result = dynamic_optimal_path::find_optimal_path(brightness, maxStops);
		auto optimalPath = result.first;
		int optimalMelodyCount = result.second;

		auto resultBruteForce = optimal_path_brute_force(brightness, maxStops);
		auto bruteForcePaths = resultBruteForce.first;
		int bruteForceMelodyCount = resultBruteForce.second;

		REQUIRE(optimalMelodyCount == bruteForceMelodyCount);

		bool found = std::any_of(
		    bruteForcePaths.begin(), bruteForcePaths.end(),
		    [&optimalPath](const std::vector<std::size_t> &path) { return path == optimalPath; });

		REQUIRE(found);
	}
}

// ------- helper function implementation -------

std::pair<std::vector<std::vector<std::size_t>>, int>
optimal_path_brute_force(const std::vector<int> &brightness, int maxStops) {
	std::size_t n = brightness.size();
	int minMelodyCount = INT_MAX;
	std::vector<std::vector<std::size_t>> bestPaths;

	std::function<void(std::vector<std::size_t> &, int, std::size_t)> dfs =
	    [&](std::vector<std::size_t> &path, int melodyCount, std::size_t currentIndex) {
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

		    for (std::size_t next = currentIndex + 1;
		         next < std::min(currentIndex + maxStops + 1, n); ++next) {
			    int newMelodyCount = melodyCount;
			    if (brightness[currentIndex] <= brightness[next]) {
				    newMelodyCount++;
			    }
			    path.push_back(next);
			    dfs(path, newMelodyCount, next);
			    path.pop_back();
		    }
	    };

	std::vector<std::size_t> path = {0};
	dfs(path, 0, 0);

	return {bestPaths, minMelodyCount};
}
