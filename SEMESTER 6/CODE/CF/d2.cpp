#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;
const int MAXN = 100001;
const int LOG = 20;

class Solution {
public:
    vector<int> parent, depth, arr;
    vector<vector<int>> up, graph;

    void dfs(int node, int par, int dep) {
        parent[node] = par;
        depth[node] = dep;
        up[node][0] = par;
        
        for (int j = 1; j < LOG; j++) {
            if (up[node][j-1] != -1)up[node][j] = up[up[node][j-1]][j-1];
            
        }
        for (int child : graph[node]) {
            if (child != par) dfs(child, node, dep + 1);
        }
    }

    int lca(int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);
        for (int j = LOG-1; j >= 0; j--) {
            if (depth[u] - (1 << j) >= depth[v]) u = up[u][j];
            
        }
        if (u == v) return u;
        for (int j = LOG-1; j >= 0; j--) {
            if (up[u][j] != up[v][j]) {
                u = up[u][j];
                v = up[v][j];
            }
        }
        return parent[u];
    }

    long long path_sum(int u, int v) {
        int lca_node = lca(u, v);
        long long sum = 0;
        
        while (u != lca_node) {
            sum = (sum + arr[u]) % MOD;
            u = parent[u];
        }
        
        while (v != lca_node) {
            sum = (sum + arr[v]) % MOD;
            v = parent[v];
        }
        
        sum = (sum + arr[lca_node]) % MOD;
        return sum;
    }

    vector<int> pathQuery(vector<int>& input_arr, vector<pair<int,int>>& edges, vector<vector<int>>& queries) {
        int n = input_arr.size();
        
        parent.resize(n, -1);
        depth.resize(n, 0);
        arr = input_arr;  
        graph.resize(n);
        up.assign(n, vector<int>(LOG, -1));
        
        for (auto& edge : edges) {
            graph[edge.first].push_back(edge.second);
            graph[edge.second].push_back(edge.first);
        }
        
        dfs(0, -1, 0);
        
        long long total_sum = 0;
        for (int val : arr) {
            total_sum = (total_sum + val) % MOD;
        }
        
        vector<int> result;
        for (auto& query : queries) {
            int u = query[0], v = query[1], x = query[2];
            
            long long path_sum_value = path_sum(u, v);
            
            long long query_sum = total_sum;
            query_sum = (query_sum - path_sum_value + MOD) % MOD;
            query_sum = (query_sum + path_sum_value * x) % MOD;
            
            result.push_back(query_sum);
        }
        return result;
    }
};

int main() {
    Solution solver;
    vector<int> arr = {1, 2, 3};
    vector<pair<int,int>> edges = {{0, 1}, {0, 2}};
    vector<vector<int>> queries = {{0, 2, 2}, {1, 2, 3}};
    vector<int> result = solver.pathQuery(arr, edges, queries);
    for (int res : result) {
        cout << res << " ";  
    }
    cout << endl;

    return 0;
}