#include <iostream>
#include <vector>

using namespace std;

// Function to build the hierarchy tree
void buildHierarchyTree(const vector<int>& arr, vector<vector<int>>& tree) {
    int n = arr.size();
    for (int i = 0; i < n; ++i) {
        tree[arr[i]].push_back(i + 1); // Knight i+1 reports to arr[i]
    }
}

// DFS to calculate the maximum number of squads
int dfs(int node, const vector<vector<int>>& tree, int& squadCount) {
    int subtreeSize = 0;

    // Traverse all children
    for (int child : tree[node]) {
        subtreeSize += dfs(child, tree, squadCount);
    }

    // Pair knights in the current subtree
    squadCount += subtreeSize / 2;

    // Return 1 if there's an unpaired knight, else 0
    return subtreeSize % 2 == 0 ? 1 : 0;
}

// Function to calculate the maximum number of two-knight squads
int maxTwoKnightSquads(const vector<int>& arr) {
    int n = arr.size() + 1; // Total knights including the King
    vector<vector<int>> tree(n);

    // Build the hierarchy tree
    buildHierarchyTree(arr, tree);

    int squadCount = 0;
    dfs(0, tree, squadCount); // Start DFS from the King (node 0)

    return squadCount;
}

int main() {
    // Test cases
    vector<int> arr = {0, 1, 2}; // Expected 0
    cout << "Maximum squads: " << maxTwoKnightSquads(arr) << endl;

    arr = {0, 1, 0}; // Expected 1
    cout << "Maximum squads: " << maxTwoKnightSquads(arr) << endl;

    arr = {0, 0, 2, 1, 1, 3}; // Expected 3
    cout << "Maximum squads: " << maxTwoKnightSquads(arr) << endl;

    return 0;
}
