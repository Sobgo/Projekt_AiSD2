#include <catch2/catch_test_macros.hpp>
#include <vector>

#include "../src/bipartite_maximum_matching_lib/bipartite_maximum_matching.hpp"

// we want maximum matching but it's hard to check easily
bool is_maximal_matching(const std::vector<std::pair<size_t, size_t>> &matching, const std::vector<std::pair<size_t, size_t>> &edges);

TEST_CASE("bipartite_maximum_matching empty", "[bipartite_maximum_matching]") {
	std::vector<std::pair<size_t, size_t>> in = {};
	auto result = bipartite_maximum_matching(in);
	REQUIRE(is_maximal_matching(result, in));
}

TEST_CASE("bipartite_maximum_matching 1 pair", "[bipartite_maximum_matching]") {
	std::vector<std::pair<size_t, size_t>> in = {{0, 0}};
	auto result = bipartite_maximum_matching(in);
	REQUIRE(is_maximal_matching(result, in));
}

TEST_CASE("bipartite_maximum_matching 1 pair sparse", "[bipartite_maximum_matching]") {
	std::vector<std::pair<size_t, size_t>> in = {{100, 1000}};
	auto result = bipartite_maximum_matching(in);
	REQUIRE(is_maximal_matching(result, in));
}

TEST_CASE("bipartite_maximum_matching lista3zad2", "[bipartite_maximum_matching]") {
	std::vector<std::pair<size_t, size_t>> in = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 1},
	                                             {1, 4}, {2, 1}, {2, 2}, {3, 3}, {4, 2}, {4, 3}};
	auto result = bipartite_maximum_matching(in);
	REQUIRE(is_maximal_matching(result, in));
}

TEST_CASE("bipartite_maximum_matching lista3zad1", "[bipartite_maximum_matching]") {
	std::vector<std::pair<size_t, size_t>> in = {{0, 0}, {0, 1}, {1, 2}, {2, 1}, {2, 2}, {2, 4}, {2, 5}, {3, 3}, {3, 5}, {4, 2}, {4, 4}};
	std::vector<std::pair<size_t, size_t>> out = {{0, 0}, {1, 2}, {2, 1}, {3, 3}, {4, 4}};
	auto result = bipartite_maximum_matching(in);
	REQUIRE(result == out);
	REQUIRE(is_maximal_matching(result, in));
}

TEST_CASE("bipartite_maximum_matching medium", "[bipartite_maximum_matching]") {
	std::vector<std::pair<size_t, size_t>> in = {
	#include "input_data/bipartite_maximum_matching_medium.txt"
	};
	auto result = bipartite_maximum_matching(in);
	REQUIRE(is_maximal_matching(result, in));
}

TEST_CASE("bipartite_maximum_matching large", "[bipartite_maximum_matching]") {
	std::vector<std::pair<size_t, size_t>> in = {
	#include "input_data/bipartite_maximum_matching_large.txt"
	};
	auto result = bipartite_maximum_matching(in);
	REQUIRE(is_maximal_matching(result, in));
}

bool is_maximal_matching(const std::vector<std::pair<size_t, size_t>> &matching, const std::vector<std::pair<size_t, size_t>> &edges) {
	size_t left_num = 0;
	size_t right_num = 0;
	for (auto &pair : matching) {
		left_num = std::max(left_num, pair.first + 1);
		right_num = std::max(right_num, pair.second + 1);
	}
	for (auto &edge : edges) {
		left_num = std::max(left_num, edge.first + 1);
		right_num = std::max(right_num, edge.second + 1);
	}

	std::vector<bool> left_present(left_num, false);
	std::vector<bool> right_present(right_num, false);
	for (auto &pair : matching) {
		if (left_present[pair.first] || right_present[pair.second]) {
			return false;
		}
		left_present[pair.first] = true;
		right_present[pair.second] = true;
	}

	for (auto &edge : edges) {
		if (!left_present[edge.first] && !right_present[edge.second]) {
			return false;
		}
	}

	return true;
}
