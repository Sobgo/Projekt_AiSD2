#include <catch2/catch_test_macros.hpp>
#include "../src/bipartite_maximum_matching_lib/bipartite_maximum_matching.h"

TEST_CASE("bipartite_maximum_matching empty", "[bipartite_maximum_matching]") {
    REQUIRE(bipartite_maximum_matching({}) == std::vector<std::pair<size_t, size_t>>{});
}