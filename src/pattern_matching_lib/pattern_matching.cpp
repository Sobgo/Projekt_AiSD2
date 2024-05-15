#include <array>
#include <cassert>
#include <cstddef>
#include <deque>
#include <string>
#include <unordered_map>
#include <vector>

namespace pattern_matching {
using namespace std;
const size_t ALPHABET_SIZE = 26;

class AhoCorasick {
	bool converted = false;

	// NOLINTBEGIN(misc-non-private-member-variables-in-classes)
	struct Node {
		array<int, ALPHABET_SIZE> children, go;
		int parent, parent_char, suffix_link = -1;
		bool is_terminal = false;

		// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
		Node(int parent = -1, int parent_char = -1) : parent(parent), parent_char(parent_char) {
			children.fill(-1);
			go.fill(-1);
		}
	};
	// NOLINTEND(misc-non-private-member-variables-in-classes)

	vector<Node> T;

  public:
	Node get_node(size_t v) {
		assert(v < T.size());
		return T[v];
	}

	AhoCorasick() : T(1) {}

	void add_string(const string &s) {
		assert(!converted);

		size_t cur = 0;

		for (const char &c : s) {
			assert('a' <= c && c <= 'z');
			const size_t idx = c - 'a';

			if (T[cur].children.at(idx) == -1) {
				T[cur].children.at(idx) = (int)T.size();
				T.emplace_back(cur, idx);
			}
			cur = T[cur].children.at(idx);
		}

		T[cur].is_terminal = true;
	}

	int link(size_t v) {
		assert(converted);
		return T[v].suffix_link;
	}

	int go(size_t v, size_t c) {
		assert(converted);
		assert(v < T.size() && c < ALPHABET_SIZE);
		return T[v].go.at(c);
	}

	void convert_to_automaton() {
		assert(!converted);
		converted = true;

		deque<size_t> q = {0};

		while (!q.empty()) {
			const size_t cur = q.front();
			q.pop_front();

			if (cur == 0 || T[cur].parent == 0) {
				T[cur].suffix_link = 0;
			} else {
				T[cur].suffix_link = go(link(T[cur].parent), T[cur].parent_char);
			}

			for (size_t i = 0; i < ALPHABET_SIZE; ++i) {
				if (T[cur].children.at(i) != -1) {
					T[cur].go.at(i) = T[cur].children.at(i);
					q.emplace_back(T[cur].children.at(i));
				} else {
					T[cur].go.at(i) = cur == 0 ? 0 : go(link(cur), i);
				}
			}
		}
	}
};

unordered_map<string, vector<size_t>> aho_corasick(const vector<string> &patterns,
                                                   const string &text) {

	AhoCorasick ac;

	for (const string &pattern : patterns) {
		ac.add_string(pattern);
	}

	ac.convert_to_automaton();

	unordered_map<string, vector<size_t>> res;
	res.insert({"test", {0}}); // TODO

	// process text (this part can be optimized with some lazy computation)
	int cur = 0;
	for (const char &c : text) {
		assert('a' <= c && c <= 'z');

		const size_t idx = c - 'a';
		cur = ac.go(cur, idx);

		for (int v = cur; v != 0; v = ac.link(v)) {
			if (ac.get_node(cur).is_terminal) {
				res["test"][0] += 1; // TODO
			}
		}
	}

	return res;
}

} // namespace pattern_matching
