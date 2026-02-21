
#include <stdio.h>
#include <stdlib.h>

#define MAX_FRAMES 100


void LRU_Page_Replacement(int num_frames, int *page_requests, int num_requests) {
    int frames[MAX_FRAMES];  // Array to hold the pages in memory
    int front = 0;  // Pointer to the front of the frame array (for LRU eviction)
    int rear = 0;   // Pointer to track the next available slot in the frame array
    int page_faults = 0;

    // Initially, no page is in memory
    for (int i = 0; i < num_frames; i++) {
        frames[i] = -1;  // -1 means empty
    }

    // Simulate page requests
    for (int i = 0; i < num_requests; i++) {
        
        int page = page_requests[i];
        
        int page_found = 0;

        // Check if the page is already in the frames (hit)
        for (int j = 0; j < num_frames; j++) {
            if (frames[j] == page) {
                page_found = 1;
                break;
            }
        }

        // If the page was not found in memory (page fault)
        if (!page_found) {
            
            page_faults++;
            
            if (rear < num_frames) {
                // There's space in memory, add the page to the next available frame
                frames[rear] = page;
                rear++;
            } 
            else {
                // Memory is full, evict the least recently used page (from the front)
                for (int j = 0; j < num_frames - 1; j++) {
                    frames[j] = frames[j + 1];  // Shift frames to the left
                }
                frames[num_frames - 1] = page;  // Add the new page to the last position
            }
        }

        // Print the current state of frames
        printf("P%d -> PF      Frames: [", page);
        for (int j = 0; j < num_frames; j++) {
            if (frames[j] != -1) {
                printf("%d", frames[j]);
                if (j < num_frames - 1 && frames[j + 1] != -1) {
                    printf(", ");
                }
            }
        }
        printf("]\n");
    }

    printf("\nTotal page faults: %d\n\n", page_faults);
}

int main() {
    int num_frames;
    int page_requests[MAX_FRAMES];
    int num_requests = 0;

    // Input: Number of frames
    printf("Enter the number of frames: ");
    scanf("%d", &num_frames);

    // Input: Sequence of page requests
    printf("Enter the sequence of page requests (space separated): ");
    while (scanf("%d", &page_requests[num_requests]) == 1) {
        num_requests++;
        char ch = getchar();
        if (ch == '\n') {
            break; // Stop when a new line is encountered
        }
    }

    printf("\n"); 

    // Simulate the LRU page replacement algorithm
    LRU_Page_Replacement(num_frames, page_requests, num_requests);

    return 0;
}



// Enter the number of frames: 3
// Enter the sequence of page requests (space separated): 7 0 1 2 0 3 0 4 2 3 0 3 2
