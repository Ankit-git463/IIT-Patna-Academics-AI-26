#include <iostream>
#include <vector>
using namespace std;

const int LOG = 20; 

// Binary lifting table
vector<vector<int>> up;

void preprocess(int n) {
    

    // Build the binary lifting table
    for (int j = 1; j < LOG; ++j) {
        for (int i = 1; i <= n; ++i) {
            if (up[i][j - 1] != -1) {
                up[i][j] = up[up[i][j - 1]][j - 1];
            }
        }
    }
}

// Process a query to find the k-th ancestor of employee x
int query(int x, int k) {
    for (int j = 0; j < LOG; ++j) {
        
        if (k & (1 << j)) { 
            x = up[x][j];

            // No such ancestor exists
            if (x == -1) return -1; 
        }
    }
    return x;
}

int main() {
    int n, q;
    cin >> n >> q;
    
    up.assign(n + 1, vector<int>(LOG, -1)); 

    for (int i = 2; i <= n; ++i) {
        cin>> up[i][0];
    }
  

    preprocess(n);

    
    for (int i = 0; i < q; ++i) {
        int x, k;
        cin >> x >> k;
        cout << query(x, k) << endl;
    }

    return 0;
}
