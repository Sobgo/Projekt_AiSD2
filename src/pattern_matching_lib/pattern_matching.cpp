#include <array>
#include <cassert>
#include <cstddef>
#include <deque>
#include <limits>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

/**
 * @brief Pattern Matching
 */
namespace pattern_matching {
using namespace std;

class AhoCorasick {
	size_t ALPHABET_SIZE;

	struct Node {
		vector<optional<size_t>> children, go;
		optional<size_t> parent, parent_char, suffix_link = nullopt, exit_link = nullopt;

		bool is_terminal = false;
		vector<size_t> terminal_ids;

		Node(size_t ALPHABET_SIZE, optional<size_t> parent = nullopt,
		     optional<size_t> parent_char = nullopt)
		    : parent(parent), parent_char(parent_char) {
			children.assign(ALPHABET_SIZE, nullopt);
			go.assign(ALPHABET_SIZE, nullopt);
		}
	};

	vector<Node> T;
	bool converted = false;

  public:
	AhoCorasick(size_t alphabet_size) : T(1, Node(alphabet_size)), ALPHABET_SIZE(alphabet_size) {}
	Node get_node(size_t v) { return T[v]; }

	void add_string(const vector<size_t> &s, size_t idx) {
		assert(!converted);

		size_t cur = 0;
		for (const size_t &idx : s) {
			auto child_node = T[cur].children.at(idx);

			if (!child_node.has_value()) {
				child_node = T.size();
				T[cur].children.at(idx) = T.size();
				T.emplace_back(ALPHABET_SIZE, cur, idx);
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
 * @param alphabet string of characters that can appear in the text
 * @param text string of alphabet characters to search in
 * @param patterns vector of alphabet characters strings to search for in the text
 * @returns a vector of vectors where the i-th vector contains the starting indices of all
 * occurrences of the i-th pattern in the text.
 */
vector<vector<size_t>> aho_corasick(const string &alphabet, const string &text,
                                    const vector<string> &patterns) {

	array<optional<size_t>, numeric_limits<unsigned char>::max() + 1> M;

	size_t iter = 0;
	for (const char &c : alphabet) {
		if (M.at(c).has_value()) continue;
		M.at(c) = iter;
		++iter;
	}

	AhoCorasick ac(iter);

	for (size_t i = 0; i < patterns.size(); ++i) {
		vector<size_t> pattern;
		for (const char &c : patterns[i]) {
			const auto &c_val = M.at(c);

			if (!c_val.has_value()) {
				string msg = "Pattern character \"";
				msg += c;
				msg += "\" not in alphabet.";
				throw invalid_argument(msg);
			}

			pattern.push_back(c_val.value());
		}

		ac.add_string(pattern, i);
	}

	ac.convert_to_automaton();

	vector<vector<size_t>> res(patterns.size());

	size_t cur = 0;
	for (size_t i = 0; i < text.size(); ++i) {
		const char &c = text[i];

		const auto &c_val = M.at(c);
		if (!c_val.has_value()) {
			string msg = "Text character \"";
			msg += c;
			msg += "\" not in alphabet.";
			throw invalid_argument(msg);
		}

		const size_t idx = c_val.value();
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

}
