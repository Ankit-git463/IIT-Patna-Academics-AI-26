#include <iostream>
#include <vector>
#include <cmath>
using namespace std;


vector< vector <int>> dp;
int n ; 
int K ; 

// Build the Sparse Table
void build(vector<int>& arr) {
   
    // Base case: intervals of size 2^0 (single elements)
    for (int i = 0; i < n; i++) {
        dp[i][0] = arr[i];
    }

    //Sparse Table for intervals of size 2^j
    for (int j = 1; (1 << j) <= n; j++) {
        // i ----->>> 2^j 
        for (int i = 0; i + (1 << j) <= n; i++) {
            dp[i][j] = min(dp[i][j - 1], dp[i + (1 << (j - 1))][j - 1]);
        }
    }
}

// Query the Sparse Table for RMQ in range [L, R]
int query( int L, int R) {
    int j = log2(R - L + 1);
    return min(dp[L][j], dp[R - (1 << j) + 1][j]);
}

// Example Usage
int main() {
    vector<int> arr = {1, 3, -1, 7, 0, 3, 4};
    build(arr);
    n = arr.size();
    K = log2(n) + 1;

    dp.assign(n , vector <int> (K, -1 ));

    int L = 1, R = 4;
    cout << query( L, R) << endl;

    return 0;
}
