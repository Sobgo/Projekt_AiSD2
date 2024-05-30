#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>

#include "../bipartite_maximum_matching_lib/bipartite_maximum_matching.hpp"

using namespace std;

int main() {
	vector<pair<size_t, size_t>> input;
	size_t a = 0, b = 0;
	while (cin >> a >> b) {
		input.emplace_back(a - 1, b - 1);
	}
	auto result = bipartite_maximum_matching::bipartite_maximum_matching(input);
	for (auto &p : result) {
		cout << p.first + 1 << " " << p.second + 1 << '\n';
	}

	return 0;
}
