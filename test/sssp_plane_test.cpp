#include <catch2/catch_test_macros.hpp>
#include <vector>

#include "../src/sssp_plane_lib/sssp_plane.hpp"

TEST_CASE("sssp_plane empty", "[sssp_plane]") {
	REQUIRE_THROWS(sssp_plane::sssp_plane({}, {}, 0, {}));
}

TEST_CASE("sssp_plane single point", "[sssp_plane]") {
	auto result = sssp_plane::sssp_plane({{0, 0}}, {}, 0, {0});
	REQUIRE(result.size() == 1);
	REQUIRE(result[0].first == 0);
	REQUIRE(result[0].second.empty());
}

TEST_CASE("sssp_plane single edge", "[sssp_plane]") {
	auto result = sssp_plane::sssp_plane({{0, 0}, {1, 0}}, {{0, 1}}, 0, {1});
	REQUIRE(result.size() == 1);
	REQUIRE(result[0].first == 1);
	REQUIRE(result[0].second.size() == 1);
	REQUIRE(result[0].second[0] == 1);
}
