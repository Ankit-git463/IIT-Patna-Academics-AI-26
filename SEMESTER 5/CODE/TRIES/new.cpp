#include <bits/stdc++.h>
// Calculate number of matching indices between two inventories
int calculateSimilarity(const std::vector<int>& inv1, const std::vector<int>& inv2) {
    int similarity = 0;
    for (int i = 0; i < inv1.size(); ++i) {
        if (inv1[i] == inv2[i]) {
            similarity++;
        }
    }
    return similarity;
}
int getMaxEqualIndices(std::vector<int>& inv1, std::vector<int>& inv2) {
    int n = inv1.size();
    int maxSimilarity = 0;
    std::vector<int> currentInv1 = inv1;
    maxSimilarity = calculateSimilarity(currentInv1, inv2);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j || currentInv1[j] <= 0) continue;
            std::vector<int> testInv1 = currentInv1;
            testInv1[i]++;
            testInv1[j]--;
            
            int newSimilarity = calculateSimilarity(testInv1, inv2);
            maxSimilarity = std::max(maxSimilarity, newSimilarity);
            for (int k = 0; k < n; ++k) {
                for (int l = 0; l < n; ++l) {
                    if (k == l || testInv1[l] <= 0) continue;
                    
                    std::vector<int> finalInv1 = testInv1;
                    finalInv1[k]++;
                    finalInv1[l]--;
                    
                    newSimilarity = calculateSimilarity(finalInv1, inv2);
                    maxSimilarity = std::max(maxSimilarity, newSimilarity);
                }
            }
        }
    }
    
    return maxSimilarity;
}




// Example usage function
int main() {
    
    
    // Test Case 1
    std::vector<int> inv1_1 = {2, 2};
    std::vector<int> inv2_1 = {3, 1};
    std::cout << "Test Case 1 Result: " << getMaxEqualIndices(inv1_1, inv2_1) << std::endl; // Expected: 2
    
    // Test Case 2
    std::vector<int> inv1_2 = {3, 2, 1};
    std::vector<int> inv2_2 = {2, 1, 4};
    std::cout << "Test Case 2 Result: " << getMaxEqualIndices(inv1_2, inv2_2) << std::endl; // Expected: 2
    
    return 0;
}