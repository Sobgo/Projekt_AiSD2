#include <catch2/catch_test_macros.hpp>
#include <vector>

#include "../src/sssp_plane_lib/sssp_plane.hpp"

TEST_CASE("sssp_plane empty", "[sssp_plane]") {
	REQUIRE_THROWS(sssp_plane::sssp_plane({}, {}, 0, {}));
}

TEST_CASE("sssp_plane single point", "[sssp_plane]") {
	auto result = sssp_plane::sssp_plane({{0, 0}}, {}, 0, {0});
	std::vector<sssp_plane::SSSP_Path> expected = {sssp_plane::SSSP_Path(0, {0}, 0)};
	REQUIRE(result == expected);
}

TEST_CASE("sssp_plane single edge", "[sssp_plane]") {
	auto result = sssp_plane::sssp_plane({{0, 0}, {1, 0}}, {{0, 1}}, 0, {1});
	std::vector<sssp_plane::SSSP_Path> expected = {sssp_plane::SSSP_Path(1, {0, 1}, 1)};
	REQUIRE(result == expected);
}
