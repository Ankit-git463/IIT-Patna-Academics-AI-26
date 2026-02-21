#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    long long countPaths(vector<vector<long long>>& grid, long long k) {
        int n = grid.size();
        int m = grid[0].size();

        // DP table to store the number of ways to reach each cell with a cost <= k
        vector<vector<long long>> dp(n, vector<long long>(m, 0));

        // Base case: Start from the top-left corner
        if (grid[0][0] <= k) {
            dp[0][0] = 1;
        }

        // Fill the DP table
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (grid[i][j] == -1) {
                    continue; // Skip blocked cells
                }

                // Transition from the cell above
                if (i > 0 && grid[i][j] + grid[i-1][j] <= k) {
                    dp[i][j] += dp[i-1][j];
                }

                // Transition from the cell to the left
                if (j > 0 && grid[i][j] + grid[i][j-1] <= k) {
                    dp[i][j] += dp[i][j-1];
                }
            }
        }

        // Return the number of valid paths to the bottom-right corner
        return dp[n-1][m-1];
    }
};

int main() {
    Solution solution;
    
    // Test case 1
    vector<vector<long long>> grid1 = {{1, 2}, {3, 4}};
    long long k1 = 5;
    cout << "Test Case 1: " << solution.countPaths(grid1, k1) << endl; // Expected Output: 0

    // Test case 2
    vector<vector<long long>> grid2 = {{1, 2}, {3, 4}};
    long long k2 = 8;
    cout << "Test Case 2: " << solution.countPaths(grid2, k2) << endl; // Expected Output: 2

    // Test case 3: Edge case with a larger grid
    vector<vector<long long>> grid3 = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
    long long k3 = 4;
    cout << "Test Case 3: " << solution.countPaths(grid3, k3) << endl; // Expected Output: 6

    // Test case 4: Edge case with blocked cells
    vector<vector<long long>> grid4 = {{1, -1}, {3, 4}};
    long long k4 = 5;
    cout << "Test Case 4: " << solution.countPaths(grid4, k4) << endl; // Expected Output: 0

    return 0;
}
