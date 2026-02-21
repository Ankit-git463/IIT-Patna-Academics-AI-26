#include <vector>
#include <unordered_map>
#include <iostream>

class Solution {
public:
    // Binary Indexed Tree (Fenwick Tree) for efficient range sum queries
    class BIT {
        std::vector<int> bit;
    public:
        BIT(int size) : bit(size + 1, 0) {}
        
        void update(int idx, int val) {
            idx++; // 1-based indexing
            while (idx < bit.size()) {
                bit[idx] += val;
                idx += idx & (-idx);
            }
        }
        
        int query(int idx) {
            idx++; // 1-based indexing
            int sum = 0;
            while (idx > 0) {
                sum += bit[idx];
                idx -= idx & (-idx);
            }
            return sum;
        }
    };

    int getInaccurateProcesses(std::vector<int>& processOrder, std::vector<int>& executionOrder) {
        int n = processOrder.size();
        
        // Map each process to its position in processOrder
        std::vector<int> processPosition(n + 1);
        for (int i = 0; i < n; i++) {
            processPosition[processOrder[i]] = i;
        }
        
        // Initialize BIT for tracking executed processes
        BIT bit(n);
        
        int inaccurateCount = 0;
        
        // Process each execution
        for (int i = 0; i < n; i++) {
            int currentProcess = executionOrder[i];
            int pos = processPosition[currentProcess];
            
            // Check if all required processes are executed
            if (pos > 0) {  // if process needs predecessors
                int executedBefore = bit.query(pos - 1);
                if (executedBefore < pos) {  // if not all required processes are executed
                    inaccurateCount++;
                }
            }
            
            // Mark current process as executed
            bit.update(pos, 1);
        }
        
        return inaccurateCount;
    }
};

// Test function
int main() {
    Solution solution;
    
    // Test Case 1
    std::vector<int> processOrder1 = {2, 3, 5, 1, 4};
    std::vector<int> executionOrder1 = {5, 2, 3, 4, 1};
    std::cout << "Test Case 1 Result: " 
              << solution.getInaccurateProcesses(processOrder1, executionOrder1) 
              << std::endl; // Expected: 2
    
    // Test Case 2
    std::vector<int> processOrder2 = {1, 2, 3, 4, 5};
    std::vector<int> executionOrder2 = {5, 4, 3, 2, 1};
    std::cout << "Test Case 2 Result: " 
              << solution.getInaccurateProcesses(processOrder2, executionOrder2) 
              << std::endl; // Expected: 4
    
    // Large Test Case
    int n = 200000;
    std::vector<int> processOrder3(n);
    std::vector<int> executionOrder3(n);
    for (int i = 0; i < n; i++) {
        processOrder3[i] = i + 1;
        executionOrder3[i] = n - i;
    }
    std::cout << "Large Test Case Result: " 
              << solution.getInaccurateProcesses(processOrder3, executionOrder3) 
              << std::endl;
    
    return 0;
}