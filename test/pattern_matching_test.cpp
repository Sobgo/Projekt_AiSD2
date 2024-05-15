#include <catch2/catch_test_macros.hpp>
#include <vector>

#include "../src/pattern_matching_lib/pattern_matching.hpp"

using namespace std;

SCENARIO("Pattern exists in text") {
	GIVEN("A text and a single pattern") {
		vector<string> patterns = {"ab"};
		string text = "aabacbabcb";

		WHEN("The text contains the pattern multiple times") {
			auto result = pattern_matching::aho_corasick(patterns, text);

			THEN("Pattern exists at the obtained positions within the text") {
				REQUIRE(result["test"][0] == 2); // TODO
			}

			THEN("Pattern does not exist at any other position within the text") {
				// TODO
			}
		}
	}
}
