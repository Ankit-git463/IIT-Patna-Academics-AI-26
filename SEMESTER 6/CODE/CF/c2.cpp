#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Function to check if Sam can get a price of at least x
bool canGuaranteePrice(vector<int>& arr, int m, int k, int x) {
    int n = arr.size();
    int left = 0, right = n - 1;
    
    // The number of people Sam can influence is k
    // Persuade the first k people to pick either the first or last item optimally
    for (int i = 0; i < k; i++) {
        if (arr[left] >= arr[right]) {
            left++;  // Persuade to pick the left item if it's greater or equal
        } else {
            right--; // Persuade to pick the right item if it's greater
        }
    }
    
    // Now, check if Sam can guarantee an item of at least price x
    for (int i = left; i <= right; i++) {
        if (arr[i] >= x) {
            return true;  // Sam can pick this item
        }
    }
    
    return false;  // No item left with price >= x
}

// Function to determine the largest minimum price Sam can get
int guaranteePick(vector<int>& arr, int m, int k) {
    int n = arr.size();
    
    // Perform binary search on the possible values of x
    int low = 1, high = *max_element(arr.begin(), arr.end());
    int result = 0;
    
    while (low <= high) {
        int mid = low + (high - low) / 2;
        
        if (canGuaranteePrice(arr, m, k, mid)) {
            result = mid;  // We can guarantee at least price mid
            low = mid + 1; // Try for a higher price
        } else {
            high = mid - 1; // Try for a lower price
        }
    }
    
    return result;
}

int main() {
    // Example Input 1
    vector<int> arr = {3, 2, 4, 8, 7, 5};
    int m = 4, k = 2;
    cout << "Output: " << guaranteePick(arr, m, k) << endl;  // Expected output: 7

    // Example Input 2
    vector<int> arr2 = {3, 1, 2};
    m = 2, k = 2;
    cout << "Output: " << guaranteePick(arr2, m, k) << endl;  // Expected output: 3

    // Example Input 3
    vector<int> arr3 = {15, 68, 35, 65, 44, 51, 88, 9, 77, 79, 89, 85};
    m = 9, k = 7;
    cout << "Output: " << guaranteePick(arr3, m, k) << endl;  // Expected output: 88

    return 0;
}
