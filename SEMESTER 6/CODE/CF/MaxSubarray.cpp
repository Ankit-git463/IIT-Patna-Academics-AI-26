#include <iostream>
#include <vector>
using namespace std;

// Function to calculate the total number of subarrays
long long totalSubarrays(int n) {
    return (long long)n * (n + 1) / 2;
}

// Function to count subarrays with at least one 0
long long countSubarraysWithZero(const vector<int>& arr) {
    int n = arr.size();
    long long totalSubarraysCount = totalSubarrays(n);

    // Count consecutive blocks of 1s
    long long count = 0, subarraysOfOnes = 0;
    for (int num : arr) {
        if (num == 1) {
            count++;
        } else {
            subarraysOfOnes += count * (count + 1) / 2;
            count = 0;
        }
    }
    // Add remaining count for trailing ones
    subarraysOfOnes += count * (count + 1) / 2;

    return totalSubarraysCount - subarraysOfOnes;
}

int main() {
    int n;
    cin >> n;
    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    // Check if the array already contains only 0s
    bool allZeros = true;
    for (int i = 0; i < n; i++) {
        if (arr[i] == 1) {
            allZeros = false;
            break;
        }
    }

    if (allZeros) {
        // If the array is entirely 0s, return the total number of subarrays
        cout << totalSubarrays(n) << endl;
        return 0;
    }

    // Try flipping each 1 to 0 and find the maximum count of subarrays with at least one 0
    long long maxCount = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i] == 1) {
            vector<int> modifiedArr = arr;
            modifiedArr[i] = 0;
            maxCount = max(maxCount, countSubarraysWithZero(modifiedArr));
        }
    }

    cout << maxCount << endl;
    return 0;
}
