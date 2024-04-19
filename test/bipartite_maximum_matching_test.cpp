#include "../src/bipartite_maximum_matching_lib/bipartite_maximum_matching.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

TEST_CASE("bipartite_maximum_matching empty", "[bipartite_maximum_matching]") {
	std::vector<std::pair<size_t, size_t>> in = {};
	std::vector<std::pair<size_t, size_t>> out = {};
	REQUIRE(bipartite_maximum_matching(in) == out);
}

TEST_CASE("bipartite_maximum_matching 1 pair", "[bipartite_maximum_matching]") {
	std::vector<std::pair<size_t, size_t>> in = {{0, 0}};
	std::vector<std::pair<size_t, size_t>> out = {{0, 0}};
	REQUIRE(bipartite_maximum_matching(in) == out);
}

TEST_CASE("bipartite_maximum_matching 1 pair sparse", "[bipartite_maximum_matching]") {
	std::vector<std::pair<size_t, size_t>> in = {{100, 1000}};
	std::vector<std::pair<size_t, size_t>> out = {{100, 1000}};
	REQUIRE(bipartite_maximum_matching(in) == out);
}

TEST_CASE("bipartite_maximum_matching lista3zad2", "[bipartite_maximum_matching]") {
	std::vector<std::pair<size_t, size_t>> in = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 1},
	                                             {1, 4}, {2, 1}, {2, 2}, {3, 3}, {4, 2}, {4, 3}};
	std::vector<std::pair<size_t, size_t>> out = {{0, 0}, {1, 4}, {2, 1}, {3, 3}, {4, 2}};
	auto result = bipartite_maximum_matching(in);
	for (size_t i = 0; i < result.size(); i++) {
		std::cout << result[i].first << " " << result[i].second << std::endl;
	}
	REQUIRE(result == out);
}
