#include <bits/stdc++.h>
using namespace std;

class fentree {
public:
    int n;
    vector<long long> bit;

    fentree(int size) {
        n = size;
        bit.assign(n + 1, 0);
    }

    // Point update: add val to index idx
    void update(int idx, long long val) {
        while (idx <= n) {
            bit[idx] += val;
            idx += idx & -idx;
        }
    }

    // Prefix sum query: get sum from 1 to idx
    long long query(int idx) {
        long long sum = 0;
        while (idx > 0) {
            sum += bit[idx];
            idx -= idx & -idx;
        }
        return sum;
    }

    // Range update: increment all elements in range [l, r] by val
    void range_update(int l, int r, long long val) {
        update(l, val);        // Add 'val' to all elements starting from 'l'
        update(r + 1, -val);   // Subtract 'val' from elements after 'r'
    }
};

int main() {
    int n, q;
    cin >> n >> q;

    vector<long long> arr(n + 1);  // Original array, 1-based indexing
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }

    fentree bit(n);

    // Initialize the Fenwick Tree with the initial array
    for (int i = 1; i <= n; i++) {
        bit.range_update(i, i, arr[i]);  // Initially, treat each element as a range update
    }

    // Process queries
    while (q--) {
        int type;
        cin >> type;

        if (type == 1) {
            int l, r, u;
            cin >> l >> r >> u;
            bit.range_update(l, r, u);  // Range update
        } else if (type == 2) {
            int k;
            cin >> k;
            // Point query: get value at index k
            cout << bit.query(k) << endl;
        }
    }

    return 0;
}
