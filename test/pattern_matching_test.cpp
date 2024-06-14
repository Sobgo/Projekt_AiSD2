#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <random>
#include <string>
#include <vector>

#include "../src/pattern_matching_lib/pattern_matching.hpp"

using namespace std;

namespace pattern_matching_test {

vector<vector<size_t>> find_patterns(const string &text, const vector<string> &patterns);
string generate_random_string(size_t length, size_t alphabet_size);

SCENARIO("Pattern Matching: Pattern exists in text", "[pattern_matching]") {
	GIVEN("A text and a single pattern") {
		string alphabet = "abcd";
		vector<string> patterns = {"ab"};
		string text = "aabacbabcb";

		WHEN("The text contains the pattern multiple times") {
			auto result = pattern_matching::aho_corasick(alphabet, text, patterns);

			THEN("Pattern is found correctly") {
				REQUIRE((result.size() == 1 && result[0].size() == 2));
				REQUIRE(result[0][0] == 1);
				REQUIRE(result[0][1] == 6);
			}
		}
	}

	GIVEN("A text and multiple patterns") {
		string alphabet = "abc";
		vector<string> patterns = {"ab", "ba", "ca"};
		string text = "aabacbabca";

		auto result = pattern_matching::aho_corasick(alphabet, text, patterns);
		auto expected = find_patterns(text, patterns);

		THEN("All patterns are found correctly") {
			REQUIRE(result.size() == patterns.size());
			REQUIRE(result.size() == expected.size());

			for (size_t i = 0; i < patterns.size(); ++i) {
				REQUIRE(result[i] == expected[i]);
			}
		}
	}
}

SCENARIO("Pattern Matching: Pattern does not exist in text", "[pattern_matching]") {
	GIVEN("A text and a single pattern") {
		string alphabet = "abc";
		vector<string> patterns = {"bb"};
		string text = "aabacbabcb";

		WHEN("The text does not contain the pattern") {
			auto result = pattern_matching::aho_corasick(alphabet, text, patterns);

			THEN("Result for pattern is empty") {
				REQUIRE(result.size() == 1);
				REQUIRE(result[0].size() == 0);
			}
		}
	}

	GIVEN("A text and multiple patterns some of which do not exist in the text") {
		string alphabet = "abcx";
		vector<string> patterns = {"bb", "ab", "bac", "x"};
		string text = "aabacbabcb";

		auto result = pattern_matching::aho_corasick(alphabet, text, patterns);
		auto expected = find_patterns(text, patterns);

		THEN("All existing patterns are found correctly and results for non-existing patterns are "
		     "empty") {
			REQUIRE(result.size() == patterns.size());
			REQUIRE(result.size() == expected.size());

			for (size_t i = 0; i < patterns.size(); ++i) {
				REQUIRE(result[i] == expected[i]);
			}
		}
	}
}

SCENARIO("Pattern Matching: Edge cases", "[pattern_matching]") {
	GIVEN("A text and a single pattern") {
		WHEN("The pattern is a single letter") {
			string alphabet = "abc";
			vector<string> patterns = {"a"};
			string text = "aabacbabcb";
			auto result = pattern_matching::aho_corasick(alphabet, text, patterns);
			auto expected = find_patterns(text, patterns);

			THEN("Pattern is found correctly") {
				REQUIRE(result.size() == patterns.size());
				REQUIRE(result.size() == expected.size());
				REQUIRE(result[0] == expected[0]);
			}
		}

		WHEN("The text is empty") {
			string alphabet = "ab";
			vector<string> patterns = {"ab"};
			string text = "";
			auto result = pattern_matching::aho_corasick(alphabet, text, patterns);

			THEN("Result for pattern is empty") {
				REQUIRE(result.size() == 1);
				REQUIRE(result[0].size() == 0);
			}
		}

		WHEN("The pattern is empty") {
			string alphabet = "abc";
			vector<string> patterns = {""};
			string text = "aabacbabcb";
			auto result = pattern_matching::aho_corasick(alphabet, text, patterns);

			THEN("Result for pattern is empty") {
				REQUIRE(result.size() == 1);
				REQUIRE(result[0].size() == 0);
			}
		}

		WHEN("The text is a substring of the pattern") {
			string alphabet = "abc";
			vector<string> patterns = {"aabacbabcb"};
			string text = "aabac";
			auto result = pattern_matching::aho_corasick(alphabet, text, patterns);

			THEN("Result for pattern is empty") {
				REQUIRE(result.size() == 1);
				REQUIRE(result[0].size() == 0);
			}
		}

		WHEN("The pattern overlaps itslef within the text") {
			string alphabet = "a";
			vector<string> patterns = {"aaaa"};
			string text = "aaaaaaaaaaaaaaaaaaaaaaa";
			auto result = pattern_matching::aho_corasick(alphabet, text, patterns);
			auto expected = find_patterns(text, patterns);

			THEN("All occurences are found correctly") {
				REQUIRE(result.size() == patterns.size());
				REQUIRE(result[0].size() == 20);

				for (size_t i = 0; i < 20; ++i) {
					REQUIRE(result[0][i] == i);
				}
			}
		}

		WHEN("Both the pattern and the text are empty") {
			string alphabet = "";
			vector<string> patterns = {""};
			string text = "";
			auto result = pattern_matching::aho_corasick(alphabet, text, patterns);

			THEN("Result for pattern is empty") {
				REQUIRE(result.size() == 1);
				REQUIRE(result[0].size() == 0);
			}
		}

		WHEN("The patterns vector is empty") {
			string alphabet = "abc";
			vector<string> patterns = {};
			string text = "aabacbabcb";
			auto result = pattern_matching::aho_corasick(alphabet, text, patterns);

			THEN("Result is empty") { REQUIRE(result.size() == 0); }
		}

		WHEN("The text contains character not in the alphabet") {
			string alphabet = "abc";
			vector<string> patterns = {"ab"};
			string text = "aabacbabcbx";

			THEN("An exception is thrown") {
				REQUIRE_THROWS_AS(pattern_matching::aho_corasick(alphabet, text, patterns),
				                  invalid_argument);
			}
		}

		WHEN("The pattern contains character not in the alphabet") {
			string alphabet = "abc";
			vector<string> patterns = {"abx"};
			string text = "aabacbabcb";

			THEN("An exception is thrown") {
				REQUIRE_THROWS_AS(pattern_matching::aho_corasick(alphabet, text, patterns),
				                  invalid_argument);
			}
		}

		WHEN("The alphabet contains duplicate characters") {
			string alphabet = "abca";
			vector<string> patterns = {"ab"};
			string text = "aabacbabcb";

			THEN("Duplicate characters are ignored, and pattern is found correctly") {
				auto result = pattern_matching::aho_corasick(alphabet, text, patterns);
				auto expected = find_patterns(text, patterns);

				REQUIRE(result.size() == patterns.size());
				REQUIRE(result.size() == expected.size());
				REQUIRE(result[0] == expected[0]);
			}
		}
	}
}

SCENARIO("Pattern Matching: Randomized tests", "[pattern_matching]") {
	string alphabet = "abcdefghijklmnopqrstuvwxyz";

	for (size_t i = 0; i < 1000; ++i) {
		size_t textlen = 1000, keylen = i % 10 + 1, alphabet_size = i % 25 + 1;

		string text = generate_random_string(textlen, alphabet_size);

		vector<string> patterns;
		for (size_t j = 0; j < 10; ++j) {
			patterns.push_back(generate_random_string(keylen, alphabet_size));
		}

		auto result = pattern_matching::aho_corasick(alphabet, text, patterns);
		auto expected = find_patterns(text, patterns);

		REQUIRE(result.size() == patterns.size());
		REQUIRE(result.size() == expected.size());

		for (size_t i = 0; i < patterns.size(); ++i) {
			REQUIRE(result[i] == expected[i]);
		}
	}
}

// ------- helper functions implementation -------

string generate_random_string(size_t length, size_t alphabet_size) {
	string s = "";

	uniform_int_distribution<int> dis(0, alphabet_size - 1);
	default_random_engine gen(time(NULL));

	for (size_t i = 0; i < length; ++i) {
		s.push_back('a' + dis(gen));
	}

	return s;
}

/*
 * @details Naive implementation of pattern matching for testing purposes
 */
vector<vector<size_t>> find_patterns(const string &text, const vector<string> &patterns) {

	vector<vector<size_t>> res(patterns.size());

	for (size_t i = 0; i < patterns.size(); ++i) {
		if (patterns[i].empty()) {
			res[i] = {};
			continue;
		}

		vector<size_t> occ;

		size_t found = text.find(patterns[i]);
		while (found != string::npos) {
			occ.push_back(found);
			found = text.find(patterns[i], found + 1);
		}

		res[i] = occ;
	}

	return res;
}

}
