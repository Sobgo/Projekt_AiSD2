#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <cstddef>
#include <set>
#include <vector>

#include "../src/sssp_plane_lib/sssp_plane.hpp"

bool is_valid_path(const sssp_plane::SSSP_Path &path, const std::vector<sssp_plane::Point> &points,
                   const std::vector<sssp_plane::Edge> &edges, size_t start);

TEST_CASE("sssp_plane empty", "[sssp_plane]") {
	REQUIRE_THROWS(sssp_plane::sssp_plane({}, {}, 0, {}));
}

TEST_CASE("sssp_plane single point", "[sssp_plane]") {
	auto result = sssp_plane::sssp_plane({{0, 0}}, {}, 0, {0});
	std::vector<sssp_plane::SSSP_Path> expected = {sssp_plane::SSSP_Path(0, {0}, 0)};
	REQUIRE(result == expected);
	REQUIRE(is_valid_path(result[0], {{0, 0}}, {}, 0));
}

TEST_CASE("sssp_plane single edge", "[sssp_plane]") {
	auto result = sssp_plane::sssp_plane({{0, 0}, {1, 0}}, {{0, 1}}, 0, {1});
	std::vector<sssp_plane::SSSP_Path> expected = {sssp_plane::SSSP_Path(1, {0, 1}, 1)};
	REQUIRE(result == expected);
	for (auto &path : result) {
		REQUIRE(is_valid_path(path, {{0, 0}, {1, 0}}, {{0, 1}}, 0));
	}
}

TEST_CASE("sssp_plane simple", "[sssp_plane]") {
	std::vector<sssp_plane::Point> points = {{0, 0}, {1, 1}, {2, 1}, {0, 1}};
	std::vector<sssp_plane::Edge> edges = {{1, 0}, {2, 1}, {3, 1}};
	std::size_t start = 2;
	std::vector<std::size_t> destinations = {0, 1};

	auto result = sssp_plane::sssp_plane(points, edges, start, destinations);
	std::vector<sssp_plane::SSSP_Path> expected = {
	    sssp_plane::SSSP_Path(0, {2, 1, 0}, 1 + std::sqrt(2)), sssp_plane::SSSP_Path(1, {2, 1}, 1)};

	REQUIRE(result == expected);
	for (auto &path : result) {
		REQUIRE(is_valid_path(path, points, edges, start));
	}
}

TEST_CASE("sssp_plane multiple_same_len", "[sssp_plane]") {
	std::vector<sssp_plane::Point> points = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
	std::vector<sssp_plane::Edge> edges = {{0, 1}, {0, 3}, {3, 2}, {1, 2}};
	std::size_t start = 0;
	std::vector<std::size_t> destinations = {2};

	auto result = sssp_plane::sssp_plane(points, edges, start, destinations);

	REQUIRE(!result.empty());
	REQUIRE(result[0].distance == 2);
	REQUIRE(is_valid_path(result[0], points, edges, start));
}

TEST_CASE("sssp_plane unreachable", "[sssp_plane]") {
	std::vector<sssp_plane::Point> points = {{0, 0}, {1, 1}};
	std::vector<sssp_plane::Edge> edges = {};
	std::size_t start = 0;
	std::vector<std::size_t> destinations = {1};

	auto result = sssp_plane::sssp_plane(points, edges, start, destinations);

	REQUIRE(result.empty());
}

TEST_CASE("sssp_plane unreachable_reverse", "[sssp_plane]") {
	std::vector<sssp_plane::Point> points = {{0, 0}, {1, 1}};
	std::vector<sssp_plane::Edge> edges = {{1, 0}};
	std::size_t start = 0;
	std::vector<std::size_t> destinations = {1};

	auto result = sssp_plane::sssp_plane(points, edges, start, destinations);

	REQUIRE(result.empty());
}

double euclidian_distance(const sssp_plane::Point &a, const sssp_plane::Point &b) {
	double dx = a.first - b.first;
	double dy = a.second - b.second;
	return std::sqrt(dx * dx + dy * dy);
}

bool is_valid_path(const sssp_plane::SSSP_Path &path, const std::vector<sssp_plane::Point> &points,
                   const std::vector<sssp_plane::Edge> &edges, const size_t start) {
	if (path.path.empty()) {
		return false;
	}
	if (path.path.size() == 1 && path.distance != 0) {
		return false;
	}
	if (path.path.front() != start) {
		return false;
	}
	std::set<sssp_plane::Edge> edges_set;
	for (const auto &edge : edges) {
		edges_set.insert(edge);
	}

	double distance = 0;
	for (size_t i = 0; i < path.path.size() - 1; ++i) {
		if (edges_set.find({path.path[i], path.path[i + 1]}) == edges_set.end()) {
			return false;
		}
		distance += euclidian_distance(points[path.path[i]], points[path.path[i + 1]]);
	}
	return distance == path.distance;
}
