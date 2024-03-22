#include "../src/test_lib.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("adder test", "[add]") {
	REQUIRE(add(1, 2) == 3);
	REQUIRE(add(3, 4) == 7);
	REQUIRE(add(5, 6) == 11);
}

TEST_CASE("this test should not fail", "[add]") { REQUIRE(add(2, 2) == 4); }
