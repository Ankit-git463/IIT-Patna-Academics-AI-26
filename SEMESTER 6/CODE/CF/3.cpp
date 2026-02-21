#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> calculateLIS(const vector<int>& arr, int N) {
    vector<int> dp(N, 1);
    for (int i = 1; i < N; i++) {
        for (int j = 0; j < i; j++) {
            if (arr[i] > arr[j]) {
                dp[i] = max(dp[i], dp[j] + 1);
            }
        }
    }
    return dp;
}

int main() {
    int N;
    cin >> N;
    vector<int> arr(N);
    for (int i = 0; i < N; i++) {
        cin >> arr[i];
    }

    vector<int> leftLIS = calculateLIS(arr, N);

    vector<int> rightLIS(N, 1);
    for (int i = N - 2; i >= 0; i--) {
        for (int j = N - 1; j > i; j--) {
            if (arr[i] < arr[j]) {
                rightLIS[i] = max(rightLIS[i], rightLIS[j] + 1);
            }
        }
    }

    int maxLIS = 0;

    for (int start = 0; start < N; start++) {
        for (int end = start; end < N; end++) {
            int currentLIS = 0;

            if (start > 0) {
                currentLIS = max(currentLIS, leftLIS[start - 1]);
            }

            int reversedLIS = end - start + 1;

            if (end < N - 1) {
                currentLIS = max(currentLIS, rightLIS[end + 1]);
            }

            maxLIS = max(maxLIS, currentLIS + reversedLIS);
        }
    }

    cout << maxLIS << endl;

    return 0;
}
