#include <catch2/catch_test_macros.hpp>
#include <vector>

#include "../src/sssp_plane_lib/sssp_plane.hpp"

TEST_CASE("sssp_plane empty", "[sssp_plane]") {
	auto result = sssp_plane({}, {}, 0, {});
	REQUIRE(result.empty());
}
