#include <algorithm>
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

bool is_lower(const string &s) {
	return all_of(s.begin(), s.end(), [](const char &c) { return 'a' <= c && c <= 'z'; });
}

class AhoCorasick {
	struct Node {
		array<optional<size_t>, ALPHABET_SIZE> children, go;
		optional<size_t> parent, parent_char, suffix_link = nullopt, exit_link = nullopt;

		bool is_terminal = false;
		vector<size_t> terminal_ids;

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
			const size_t idx = c - 'a';
			auto child_node = T[cur].children.at(idx);

			if (!child_node.has_value()) {
				child_node = T.size();
				T[cur].children.at(idx) = T.size();
				T.emplace_back(cur, idx);
			}

			cur = child_node.value();
		}

		T[cur].is_terminal = true;
		T[cur].terminal_ids.emplace_back(idx);
	}

	size_t link(size_t v) {
		assert(converted);

		const auto &link = T[v].suffix_link;
		assert(link.has_value());
		return link.value();
	}

	size_t go(size_t v, size_t c) {
		assert(converted);

		const auto &go = T[v].go.at(c);
		assert(go.has_value());
		return go.value();
	}

	size_t exit(size_t v) {
		assert(converted);

		auto cur = T[v].exit_link;
		assert(cur.has_value());

		while (T[cur.value()].exit_link != 0 && !T[cur.value()].is_terminal) {
			cur = T[cur.value()].exit_link;
			assert(cur.has_value());
		}

		T[v].exit_link = cur;
		return cur.value();
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
				const auto &parent = T[cur].parent;
				const auto &parent_char = T[cur].parent_char;
				assert((parent.has_value() && parent_char.has_value()));
				T[cur].suffix_link = go(link(parent.value()), parent_char.value());
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
		assert(is_lower(patterns[i]));
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
