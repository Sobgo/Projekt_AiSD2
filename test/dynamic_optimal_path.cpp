#include "../src/dynamic_optimal_path_lib/dynamic_optimal_path.hpp"
#include <catch2/catch_test_macros.hpp>

SCENARIO("Optimal path is found") {
	GIVEN("A set of points (short path)") {
		std::vector<std::size_t> convexHullPoints = {0, 1, 2, 3, 4, 5};
		std::vector<int> brightness = {10, 20, 30, 20, 10, 5};
		int maxStops = 2;

		auto result = find_optimal_path::optimalPath(convexHullPoints, brightness, maxStops);
		std::vector<std::size_t> expectedPath = {0, 2, 3, 5};
		int expectedMinMelodyCount = 1;

		REQUIRE(result.first == expectedPath);
		REQUIRE(result.second == expectedMinMelodyCount);
	}
}
