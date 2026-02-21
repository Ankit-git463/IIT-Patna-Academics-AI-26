#include <stdio.h> 
#include <stdlib.h> 
#define MAX_PAGES 100 


// Function to calculate page faults using FIFO algorithm 
int fifo(int pages[], int n, int frames) { 
int frame[frames]; 
int pageFaults = 0, index = 0; 
int isFull = 0; 
// Initialize frames 
for (int i = 0; i < frames; i++) { 
frame[i] = -1; 
} 
for (int i = 0; i < n; i++) { 
int j; 
// Check if page is already in frame 
for (j = 0; j < frames; j++) { 
            if (frame[j] == pages[i]) { 
                break; 
            } 
        } 
 
        // Page fault occurs if the page is not found 
        if (j == frames) { 
            frame[index] = pages[i]; 
            index = (index + 1) % frames; // Move to the next frame 
            pageFaults++; 
            if (isFull < frames) { 
                isFull++; 
            } 
        } 
    } 
    return pageFaults; 
} 
 
// Function to calculate page faults using LRU algorithm 
int lru(int pages[], int n, int frames) { 
    int frame[frames]; 
    int pageFaults = 0; 
    int isFull = 0; 
 
    // Initialize frames 
    for (int i = 0; i < frames; i++) { 
        frame[i] = -1; 
    } 
 
    for (int i = 0; i < n; i++) { 
        int j; 
        // Check if page is already in frame 
        for (j = 0; j < frames; j++) { 
            if (frame[j] == pages[i]) { 
                break; 
            } 
        } 
 
        // Page fault occurs if the page is not found 
        if (j == frames) { 
            int lruIndex = 0, lruTime = -1; 
            // Find the least recently used page 
            for (j = 0; j < frames; j++) { 
                int k; 
                for (k = i - 1; k >= 0; k--) { 
                    if (frame[j] == pages[k]) { 
                        break; 
                    } 
                } 
                if (k < lruTime) { 
                    lruTime = k; 
                    lruIndex = j; 
                } 
            } 
            frame[lruIndex] = pages[i]; // Replace the LRU page 
            pageFaults++; 
            if (isFull < frames) { 
                isFull++; 
            } 
        } 
    } 
    return pageFaults; 
} 
 
// Function to calculate page faults using Optimal algorithm 
int optimal(int pages[], int n, int frames) { 
    int frame[frames]; 
    int pageFaults = 0; 
    int isFull = 0; 
 
    // Initialize frames 
    for (int i = 0; i < frames; i++) { 
        frame[i] = -1; 
    } 
 
    for (int i = 0; i < n; i++) { 
        int j; 
        // Check if page is already in frame 
        for (j = 0; j < frames; j++) { 
            if (frame[j] == pages[i]) { 
                break; 
            } 
        } 
 
        // Page fault occurs if the page is not found 
        if (j == frames) { 
            int replaceIndex = -1, farthest = -1; 
            // Find the page to replace 
            for (j = 0; j < frames; j++) { 
                int k; 
                for (k = i + 1; k < n; k++) { 
                    if (frame[j] == pages[k]) { 
                        if (k > farthest) { 
                            farthest = k; 
                            replaceIndex = j; 
                        } 
                        break; 
                    } 
                } 
                // If page not found in future references, replace this page 
                if (k == n) { 
                    replaceIndex = j; 
                    break; 
                } 
            } 
            frame[replaceIndex] = pages[i]; // Replace the selected page 
            pageFaults++; 
            if (isFull < frames) { 
                isFull++; 
            } 
        } 
    } 
    return pageFaults; 
} 
 
int main() { 
    int n, frames; 
    int pages[MAX_PAGES]; 
 
    // User input for number of pages 
    printf("Enter the number of pages: "); 
    scanf("%d", &n); 
 
    // User input for reference string 
    printf("Enter the page reference string (space-separated): "); 
    for (int i = 0; i < n; i++) { 
        scanf("%d", &pages[i]); 
    } 
 
    // User input for number of frames 
    printf("Enter the number of frames: "); 
    scanf("%d", &frames); 
 
    // Calculate page faults for each algorithm 
    int fifoPageFaults = fifo(pages, n, frames); 
    int lruPageFaults = lru(pages, n, frames); 
    int optimalPageFaults = optimal(pages, n, frames); 
 
    printf("Total Page Faults (FIFO): %d\n", fifoPageFaults); 
    printf("Total Page Faults (LRU): %d\n", lruPageFaults); 
    printf("Total Page Faults (Optimal): %d\n", optimalPageFaults); 
 
    return 0; 
} 
