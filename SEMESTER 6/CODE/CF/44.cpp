#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <climits>

using namespace std;

// Directions: up, down, left, right
vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

// Function to perform BFS and check if a student with a given capability can visit at least K other cells
bool bfs(const vector<vector<int>>& grid, int M, int N, int x, int y, int cap, int K) {
    vector<vector<bool>> visited(M, vector<bool>(N, false));
    queue<pair<int, int>> q;
    q.push({x, y});  // Push the initial cell
    visited[x][y] = true;
    int reachable = 0;

    while (!q.empty()) {
        auto [cx, cy] = q.front();
        q.pop();
        reachable++;

        for (auto& dir : directions) {
            int nx = cx + dir.first, ny = cy + dir.second;
            if (nx >= 0 && nx < M && ny >= 0 && ny < N && !visited[nx][ny]) {
                if (abs(grid[cx][cy] - grid[nx][ny]) <= cap) {
                    visited[nx][ny] = true;
                    q.push({nx, ny});
                }
            }
        }
    }

    return reachable >= K + 1;  // Include the starting cell
}

// Function to find the minimum capability required for each cell
vector<vector<int>> minCapability(const vector<vector<int>>& grid, int M, int N, int K) {
    vector<vector<int>> result(M, vector<int>(N));

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            int low = 0, high = INT_MAX;  // Set a large upper bound
            while (low < high) {
                int mid = (low + high) / 2;
                if (bfs(grid, M, N, i, j, mid, K)) {
                    high = mid;  // Try for a smaller capability
                } else {
                    low = mid + 1;  // Increase the capability
                }
            }
            result[i][j] = low;  // The result for this cell
        }
    }

    return result;
}

int main() {
    int M, N, K;
    cin >> M >> N >> K;

    vector<vector<int>> grid(M, vector<int>(N));
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            cin >> grid[i][j];
        }
    }

    vector<vector<int>> result = minCapability(grid, M, N, K);

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            cout << result[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
