#include <array>
#include <cassert>
#include <cstddef>
#include <deque>
#include <optional>
#include <string>
#include <vector>

namespace pattern_matching {
using namespace std;
const size_t ALPHABET_SIZE = 26;

class AhoCorasick {
	struct Node {
		array<optional<size_t>, ALPHABET_SIZE> children, go;
		optional<size_t> parent, parent_char, suffix_link = nullopt, exit_link = nullopt;

		bool is_terminal = false;
		vector<size_t> terminal_ids = {};

		Node(optional<size_t> parent = nullopt, optional<size_t> parent_char = nullopt)
		    : parent(parent), parent_char(parent_char) {
			children.fill(nullopt);
			go.fill(nullopt);
		}
	};

	vector<Node> T;
	bool converted = false;

  public:
	AhoCorasick() : T(1) {}
	Node get_node(size_t v) { return T[v]; }

	void add_string(const string &s, size_t idx) {
		assert(!converted);

		size_t cur = 0;

		for (const char &c : s) {
			assert('a' <= c && c <= 'z');

			const size_t idx = c - 'a';

			if (!(T[cur].children.at(idx).has_value())) {
				T[cur].children.at(idx) = T.size();
				T.emplace_back(cur, idx);
			}

			// Guaranteed to be non-empty by the above if statement
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			cur = T[cur].children.at(idx).value();
		}

		T[cur].is_terminal = true;
		T[cur].terminal_ids.emplace_back(idx);
	}

	size_t link(size_t v) {
		assert(converted);
		assert(T[v].suffix_link.has_value());
		// Should never be nullopt if algorithm is correct
		// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
		return T[v].suffix_link.value();
	}

	size_t go(size_t v, size_t c) {
		assert(converted);
		assert(T[v].go.at(c).has_value());
		// Should never be nullopt if algorithm is correct
		// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
		return T[v].go.at(c).value();
	}

	size_t exit(size_t v) {
		assert(converted);
		assert(T[v].exit_link.has_value());
		// Should never be nullopt if algorithm is correct
		// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
		size_t cur = T[v].exit_link.value();

		while (T[cur].exit_link != 0 && !T[cur].is_terminal) {
			assert(T[cur].exit_link.has_value());
			// Should never be nullopt if algorithm is correct
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			cur = T[cur].exit_link.value();
		}

		T[v].exit_link = cur;
		return cur;
	}

	void convert_to_automaton() {
		assert(!converted);
		converted = true;

		deque<size_t> q = {0};

		while (!q.empty()) {
			const size_t cur = q.front();
			q.pop_front();

			if (cur == 0 || T[cur].parent == 0) {
				T[cur].suffix_link = T[cur].exit_link = 0;
			} else {
				assert((T[cur].parent.has_value() && T[cur].parent_char.has_value()));
				// Should never be nullopt if algorithm is correct
				// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
				T[cur].suffix_link = go(link(T[cur].parent.value()), T[cur].parent_char.value());
				T[cur].exit_link = T[cur].suffix_link;
			}

			for (size_t i = 0; i < ALPHABET_SIZE; ++i) {
				const auto &child_node = T[cur].children.at(i);

				if (child_node.has_value()) {
					T[cur].go.at(i) = child_node;
					q.emplace_back(child_node.value());
				} else {
					T[cur].go.at(i) = cur == 0 ? 0 : go(link(cur), i);
				}
			}
		}
	}
};

/**
 * Aho-Corasick algorithm for multiple pattern matching.
 * Constructs an automaton in O(sum of lengths of patterns * ALPHABET_SIZE)
 * Then the search is done in O(text length + answer length).
 * @param text string of lowercase letters to search in
 * @param patterns vector of lowercase strings to search for in the text
 * @returns a vector of vectors where the i-th vector contains the starting indices of all
 * occurrences of the i-th pattern in the text.
 */
vector<vector<size_t>> aho_corasick(const string &text, const vector<string> &patterns) {
	AhoCorasick ac;

	for (size_t i = 0; i < patterns.size(); ++i) {
		ac.add_string(patterns[i], i);
	}

	ac.convert_to_automaton();

	vector<vector<size_t>> res(patterns.size());

	size_t cur = 0;
	for (size_t i = 0; i < text.size(); ++i) {
		const char &c = text[i];
		assert('a' <= c && c <= 'z');

		const size_t idx = c - 'a';
		cur = ac.go(cur, idx);

		for (size_t v = cur; v != 0; v = ac.exit(v)) {
			const auto &node = ac.get_node(v);

			if (node.is_terminal) {
				for (const size_t &id : node.terminal_ids) {
					res[id].push_back(i - patterns[id].size() + 1);
				}
			}
		}
	}

	return res;
}

} // namespace pattern_matching
