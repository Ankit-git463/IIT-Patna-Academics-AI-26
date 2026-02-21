#include <bits/stdc++.h>
using namespace std;

const int MAXN = 2e5 + 5;
const int LOG = 20;
vector<vector<int>> uptree(MAXN, vector<int>(LOG, -1));
vector<int> depth(MAXN, 0);

// Build the binary lifting table
void build(int n) {
    for (int i = 1; i < LOG; i++) {
        for (int j = 1; j <= n; j++) {
            if (uptree[j][i - 1] != -1) {
                uptree[j][i] = uptree[uptree[j][i - 1]][i - 1];
            }
        }
    }
}

// Binary lifting to find the ancestor of `x` that is `par` levels up
int query(int x, int par) {
    for (int i = 0; i < LOG; i++) {
        if (par & (1 << i)) {
            x = uptree[x][i];
            if (x == -1) return -1;
        }
    }
    return x;
}


int lca(int x, int y) {
    
    if (depth[x] < depth[y]) swap(x, y);

    // Lift `x` to the same depth as `y`
    int diff = depth[x] - depth[y];
    x = query(x, diff);

    if (x == y) return x;

    // Lift both `x` and `y` simultaneously until they meet
    
    for (int i = LOG - 1; i >= 0; i--) {
        if (uptree[x][i] != uptree[y][i]) {
            x = uptree[x][i];
            y = uptree[y][i];
        }
    }
    return uptree[x][0];
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n, q;
    cin >> n >> q;

    vector<int> tree(n + 1, -1);

    // Read the tree input
    for (int i = 2; i <= n; i++) {
        cin >> tree[i];
        uptree[i][0] = tree[i];
        depth[i] = depth[tree[i]] + 1;  // Compute the depth while reading the tree
    }

    build(n);  // Build the binary lifting table

   
    while (q--) {
        int x, y;
        cin >> x >> y;
        cout << lca(x, y) << endl;
    }

    return 0;
}
