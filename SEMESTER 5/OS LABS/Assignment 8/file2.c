#include <stdio.h>
#include <stdbool.h>

#define np 1000
#define nr 3

// Function to find a safe sequence
bool findSafeSequence(int processes[], int n, int max[][nr], int allot[][nr], int avail[], int safeSeq[]) {
    
    int work[nr];

    bool finish[np] = {false};

    // Initialize work array with available resources
    for (int i = 0; i < nr; i++) {
        work[i] = avail[i];
    }

    int count = 0;

    while (count < n) {
        bool found = false;

        for (int p = 0; p < n; p++) {
            
            if (!finish[p]) {
                
                // Check if needs can be satisfied
                bool canAllocate = true;
                
                for (int r = 0; r < nr; r++) {
                    if (max[p][r] - allot[p][r] > work[r]) {
                        canAllocate = false;
                        break;
                    }
                }

                if (canAllocate) {
                    // Allocate resources to process
                    for (int r = 0; r < nr; r++) {
                        work[r] += allot[p][r]; // Release allocated resources
                    }

                    safeSeq[count++] = p; // Add process to safe sequence
                    finish[p] = true;     // Mark process as finished
                    found = true;
                }
            }
        }

        // If no process was found in this iteration
        if (!found) {
            printf("-1\n"); // No safe sequence found
            return false;
        }
    }

    return true; // Safe sequence found
}

int main() {
    int t;
    scanf("%d", &t); // Number of test cases

    while (t--) {
        int n;
        scanf("%d", &n); // Number of processes

        int avail[nr]; // Available resources
        scanf("%d %d %d", &avail[0], &avail[1], &avail[2]);

        int allot[np][nr];  // Allocated resources
        int max[np][nr];    // Maximum resources
        int safeSeq[np];   // To store the safe sequence

        // Input allocated and max resources for each process
        for (int i = 0; i < n; i++) {
            scanf("%d %d %d %d %d %d", &allot[i][0], &allot[i][1], &allot[i][2],
                  &max[i][0], &max[i][1], &max[i][2]);
        }

        // Find the safe sequence
        if (findSafeSequence(safeSeq, n, max, allot, avail, safeSeq)) {
           
            printf("Following is the SAFE Sequence\n");
            
            for (int i = 0; i < n; i++) {
                printf("P%d", safeSeq[i]);
                if (i < n - 1) {
                    printf(" -> ");
                }
            }

            printf("\n");
        }
    }

    return 0;
}