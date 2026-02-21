#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

class Tree {
private:
    int n, LOG;
    vector<vector<int>> adj;
    vector<vector<int>> dp; // dp[u][j] is the 2^j-th ancestor of node u
    vector<int> depth;

    void dfs(int node, int parent) {
        dp[node][0] = parent;
        for (int j = 1; j <= LOG; ++j) {
            if (dp[node][j - 1] != -1) {
                dp[node][j] = dp[dp[node][j - 1]][j - 1];
            }
        }

        for (int neighbor : adj[node]) {
            if (neighbor != parent) {
                depth[neighbor] = depth[node] + 1;
                dfs(neighbor, node);
            }
        }
    }

public:
    Tree(int n) : n(n) {
        LOG = ceil(log2(n));
        adj.resize(n);
        dp.assign(n, vector<int>(LOG + 1, -1));
        depth.assign(n, 0);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void preprocess(int root = 0) {
        depth[root] = 0;
        dfs(root, -1);
    }

    int lca(int u, int v) {
        // Ensure u is the deeper node
        if (depth[u] < depth[v]) swap(u, v);

        // Lift u up to the same depth as v
        for (int j = LOG; j >= 0; --j) {
            if (dp[u][j] != -1 && depth[dp[u][j]] >= depth[v]) {
                u = dp[u][j];
            }
        }

        if (u == v) return u; // Found LCA

        // Lift both u and v until their parent is the same
        for (int j = LOG; j >= 0; --j) {
            if (dp[u][j] != dp[v][j]) {
                u = dp[u][j];
                v = dp[v][j];
            }
        }

        return dp[u][0]; // Parent of u (or v) is the LCA
    }
};

int main() {
    int n = 9; // Number of nodes
    Tree tree(n);

    // Add edges (undirected)
    tree.addEdge(0, 1);
    tree.addEdge(0, 2);
    tree.addEdge(1, 3);
    tree.addEdge(1, 4);
    tree.addEdge(2, 5);
    tree.addEdge(2, 6);
    tree.addEdge(4, 7);
    tree.addEdge(4, 8);

    // Preprocess the tree with root as node 0
    tree.preprocess();

    // Query for LCAs
    cout << "LCA(7, 8): " << tree.lca(7, 8) << endl; // Output: 4
    cout << "LCA(3, 5): " << tree.lca(3, 5) << endl; // Output: 0
    cout << "LCA(7, 6): " << tree.lca(7, 6) << endl; // Output: 0

    return 0;
}
