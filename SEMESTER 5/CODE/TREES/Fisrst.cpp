#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

int getMaximumXORFrequency(vector<int>& arr, int val) {
    int n = arr.size();
    int maxFrequency = 0;
    
    for (int left = 0; left < n; ++left) {
        unordered_map<int, int> freqMap;
        
        for (int right = left; right < n; ++right) {
            int m = arr[right] ^ val;
            
            for (int i = left; i <= right; ++i) {
                arr[i] = arr[i] ^ m;
            }

            int currentFrequency = 0;
            for (int i = 0; i < n; ++i) {
                if (arr[i] == val) {
                    ++currentFrequency;
                }
            }

            maxFrequency = max(maxFrequency, currentFrequency);

            for (int i = left; i <= right; ++i) {
                arr[i] = arr[i] ^ m;
            }
        }
    }
    return maxFrequency;
}

int main() {
    vector<int> arr1 = {45, 12, 45, 4, 2, 2, 2};
    int val1 = 2;
    cout << getMaximumXORFrequency(arr1, val1) << endl;

    vector<int> arr2 = {1, 2, 6 , 2 , 4 , 6 , 6 ,10,10 ,10};
    int val2 = 4;
    cout << getMaximumXORFrequency(arr2, val2) << endl;

    return 0;
}
