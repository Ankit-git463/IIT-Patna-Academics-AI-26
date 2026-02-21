#include <stdio.h>
#include <stdbool.h>

#define MAX_FRAMES 10
#define MAX_PAGES 100

// Function to check if a page is present in the frames
bool isPageInFrames(int page, int frames[], int frame_count) {
    for (int i = 0; i < frame_count; i++) {
        if (frames[i] == page) {
            return true;
        }
    }
    return false;
}

int main() {
    int frames[MAX_FRAMES], page_requests[MAX_PAGES];
    int num_frames, num_requests;

    // Input the number of frames
    printf("Enter the number of frames: ");
    scanf("%d", &num_frames);

    // Initialize frames to -1 (indicating empty)
    for (int i = 0; i < num_frames; i++) {
        frames[i] = -1;
    }

    // Input the sequence of page requests
    printf("Enter the number of page requests: ");
    scanf("%d", &num_requests);
    printf("Enter the page requests: ");
    for (int i = 0; i < num_requests; i++) {
        scanf("%d", &page_requests[i]);
    }

    int page_faults = 0; // Track the number of page faults
    int next_frame_to_replace = 0; // Track the frame to replace

    // Process each page request
    for (int i = 0; i < num_requests; i++) {
        int page = page_requests[i];

        if (!isPageInFrames(page, frames, num_frames)) {
            // Page fault occurred
            page_faults++;

            // Replace the page in the next frame to replace
            int replaced_page = frames[next_frame_to_replace];
            frames[next_frame_to_replace] = page;

            // Output page fault information
            if (replaced_page == -1) {
                printf("Page %d caused a page fault. Frames: [", page);
            } else {
                printf("Page %d caused a page fault, replacing Page %d. Frames: [", page, replaced_page);
            }

            // Print the current frame contents
            for (int j = 0; j < num_frames; j++) {
                if (frames[j] != -1) {
                    printf("%d", frames[j]);
                }
                if (j < num_frames - 1) {
                    printf(", ");
                }
            }
            printf("]\n");

            // Update the index for the next frame to replace
            next_frame_to_replace = (next_frame_to_replace + 1) % num_frames;
        } else {
            // Page hit, no page fault
            printf("Page %d did not cause a page fault. Frames: [", page);
            for (int j = 0; j < num_frames; j++) {
                if (frames[j] != -1) {
                    printf("%d", frames[j]);
                }
                if (j < num_frames - 1) {
                    printf(", ");
                }
            }
            printf("]\n");
        }
    }

    printf("Total Page Faults: %d\n", page_faults);

    return 0;
}
