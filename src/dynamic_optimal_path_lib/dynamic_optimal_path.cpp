#include "dynamic_optimal_path.h"
#include <algorithm>
#include <vector>

std::vector<int> findOptimalPath(std::vector<int>& nums) {
	int n = nums.size();
	std::vector<int> dp(n, -1); // dp[i] stores the previous index that leads to index i with the minimum jumps

	dp[0] = 0;

	// Iterate through the array to fill dp table
	for (int i = 1; i < n; ++i) {
		for (int j = 0; j < i; ++j) {
			if (dp[j] != -1 && j + nums[j] >= i) { // If it's possible to reach index j and from there to index i
				if (dp[i] == -1 || dp[j] + 1 < dp[i]) {
					dp[i] = dp[j] + 1; // Update the minimum jumps needed to reach index i
				}
			}
		}
	}

	// Backtrack to find the optimal path
	std::vector<int> path;
	int index = n - 1;
	while (index != 0) {
		path.push_back(index);
		index = dp[index];
	}
	path.push_back(0); // Add the first index

	// Reverse the path to print it in the correct order
	std::reverse(path.begin(), path.end());

	return path;
}
