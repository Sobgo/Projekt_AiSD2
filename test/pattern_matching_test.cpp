#include <catch2/catch_test_macros.hpp>
#include <vector>

#include "../src/pattern_matching_lib/pattern_matching.hpp"

using namespace std;

SCENARIO("Pattern exists in text") {
	GIVEN("A text and a single pattern") {
		vector<string> patterns = {"ab"};
		string text = "aabacbabcb";

		WHEN("The text contains the pattern multiple times") {
			unordered_map<string, vector<int>> result = match(patterns, text);

			THEN("Pattern exists at the obtained positions within the text") {
				// REQUIRE(result["ab"].size() == 2);
				// REQUIRE(text.substr(result["ab"][0], patterns[0].size()) == patterns[0]);
				// REQUIRE(text.substr(result["ab"][1], patterns[0].size()) == patterns[0]);
			}

			THEN("Pattern does not exist at any other position within the text") {
				// TODO
			}
		}
	}
}
