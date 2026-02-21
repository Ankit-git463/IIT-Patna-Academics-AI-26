#include <stdio.h>
#include <stdlib.h>

#define MAX_SEGMENTS 10
#define MAX_PAGES 10

// Structure to represent each segment
typedef struct {
    int num_pages;
    int page_size;
    int *page_table; // Page table for this segment
} Segment;

// Function to initialize the segment with pages and page table
void initialize_segments(Segment *segments, int num_segments) {
    for (int i = 0; i < num_segments; i++) {
        printf("Enter number of pages for Segment %d: ", i + 1);
        scanf("%d", &segments[i].num_pages);
        
        printf("Enter page size (in bytes) for Segment %d: ", i + 1);
        scanf("%d", &segments[i].page_size);

        // Allocate memory for the page table (simulate pages in memory)
        segments[i].page_table = (int *)malloc(segments[i].num_pages * sizeof(int));

        // Initialize page table with -1 (indicating pages are not loaded initially)
        for (int j = 0; j < segments[i].num_pages; j++) {
            segments[i].page_table[j] = -1; // -1 means page is not in memory
        }
    }
}

// Function to simulate the memory loading process (handle page fault)
void load_page(int *page_table, int page_number, int page_size, int segment_number) {
    printf("Page Fault: Loading Page %d of Segment %d into memory...\n", page_number, segment_number);
    
    // Simulate loading the page by assigning a frame (just for the simulation, we use a simple frame number)
    page_table[page_number] = rand() % 1000;  // Simulate assigning a random frame
}

// Function to translate the virtual address to a physical address
void translate_address(Segment *segments, int num_segments, int segment_number, int page_number) {
    if (segment_number < 1 || segment_number > num_segments) {
        printf("Invalid segment number.\n");
        return;
    }

    segment_number--; // Convert to zero-based index
    Segment *segment = &segments[segment_number];

    // Check if the requested page is already in memory (page fault handling)
    if (page_number < 0 || page_number >= segment->num_pages) {
        printf("Invalid page number.\n");
        return;
    }

    int frame_number = segment->page_table[page_number];

    if (frame_number == -1) {
        // Page fault: the page is not loaded in memory
        load_page(segment->page_table, page_number, segment->page_size, segment_number + 1);
        frame_number = segment->page_table[page_number]; // After loading, get the new frame number
    }

    // Calculate the physical address: (frame number * page size) + offset
    int physical_address = frame_number * segment->page_size;

    // Display the result
    printf("Physical Address for Segment %d, Page %d: %d\n", segment_number + 1, page_number, physical_address);
}

int main() {
    int num_segments;

    // Input: Number of segments
    printf("Enter the number of segments: ");
    scanf("%d", &num_segments);

    if (num_segments > MAX_SEGMENTS) {
        printf("Error: Number of segments exceeds the maximum limit.\n");
        return -1;
    }

    // Initialize the segments
    Segment segments[MAX_SEGMENTS];
    initialize_segments(segments, num_segments);

    // Input: Request for a segment and page
    int segment_number, page_number;

    printf("\nEnter segment number and page number to request:\n");
    printf("Segment number: ");
    scanf("%d", &segment_number);
    printf("Page number: ");
    scanf("%d", &page_number);


    printf("\n\n") ; 
    // Translate the address
    translate_address(segments, num_segments, segment_number, page_number);

    // Free dynamically allocated memory
    for (int i = 0; i < num_segments; i++) {
        free(segments[i].page_table);
    }

    return 0;
}



// Enter the number of segments: 2
// Enter number of pages for Segment 1: 3
// Enter page size (in bytes) for Segment 1: 4
// Enter number of pages for Segment 2: 4
// Enter page size (in bytes) for Segment 2: 4

// Enter segment number and page number to request:
// Segment number: 1
// Page number: 2
