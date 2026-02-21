#include <iostream>
#include <vector>
#include <climits>
#include <unordered_set>

using namespace std;

bool checkSubsetSum(int k, const vector<int>& arr) {
    unordered_set<int> dp;
    dp.insert(0);
    
    for (int num : arr) {
        unordered_set<int> newDp(dp);
        for (int sum : dp) {
            if (sum + num <= k) {
                newDp.insert(sum + num);
            }
        }
        dp = newDp;
    }
    
    return dp.find(k) != dp.end();
}

int getMinSubarrayLength(int n, int k, vector<int>& amount) {
    int minLength = INT_MAX;

    for (int start = 0; start < n; ++start) {
        vector<int> subarray;
        for (int end = start; end < n; ++end) {
            subarray.push_back(amount[end]);
            if (checkSubsetSum(k, subarray)) {
                minLength = min(minLength, end - start + 1);
            }
        }
    }

    return (minLength == INT_MAX) ? -1 : minLength;
}

int main() {
    int n, k;
    cin >> n >> k;

    vector<int> amount(n);
    for (int i = 0; i < n; i++) {
        cin >> amount[i];
    }

    cout << getMinSubarrayLength(n, k, amount) << endl;

    return 0;
}
