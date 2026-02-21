#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int n, x;
    cin >> n >> x;
    
    vector<int> p(n);
    for (int i = 0; i < n; i++) {
        cin >> p[i];
    }
    
    // Sort the weights in ascending order
    sort(p.begin(), p.end());
    
    int i = 0;          // Pointer to the lightest child
    int j = n - 1;      // Pointer to the heaviest child
    int gondolas = 0;   // To count the number of gondolas needed
    
    while (i <= j) {
        if (p[i] + p[j] <= x) {
            // If the lightest and heaviest child can share a gondola
            i++;  // Move the pointer for the lightest child
        }
        // In both cases (paired or alone), the heaviest child will take a gondola
        j--;  // Move the pointer for the heaviest child
        gondolas++;  // We used one gondola
    }
    
    cout << gondolas << endl;
    
    return 0;
}
