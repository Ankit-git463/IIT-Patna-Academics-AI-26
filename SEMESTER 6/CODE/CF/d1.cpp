#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    int calculateXORAndOr(vector<int>& arr) {
        int n = arr.size();
        int andXor = 0, orXor = 0;

        // Process each bit position (from 0 to 31)
        for (int bit = 0; bit < 32; ++bit) {
            int andBitCount = 0, orBitCount = 0;
            
            // Calculate AND XOR for the current bit
            int currentAnd = 0xFFFFFFFF;  // Start with all bits set to 1 (AND identity)
            for (int i = 0; i < n; ++i) {
                currentAnd &= arr[i];
                if (currentAnd & (1 << bit)) {
                    andBitCount++;
                }
            }

            // Calculate OR XOR for the current bit
            int currentOr = 0;  // Start with all bits set to 0 (OR identity)
            for (int i = 0; i < n; ++i) {
                currentOr |= arr[i];
                if (currentOr & (1 << bit)) {
                    orBitCount++;
                }
            }

            // XOR the bit contributions
            if (andBitCount % 2) {
                andXor |= (1 << bit);
            }
            if (orBitCount % 2) {
                orXor |= (1 << bit);
            }
        }

        return andXor + orXor;
    }
};

// Helper function to print test cases and verify
void testCase(vector<int> arr) {
    Solution sol;
    int result = sol.calculateXORAndOr(arr);
    
    cout << "Input: ";
    for (int num : arr) {
        cout << num << " ";
    }
    cout << "\nOutput: " << result << endl;
}

int main() {
    // Test cases
    testCase({1, 2, 3});   // Expected output: 5
    testCase({1, 4, 7});   // Expected output: 13
    testCase({3});         // Expected output: 6

    return 0;
}
