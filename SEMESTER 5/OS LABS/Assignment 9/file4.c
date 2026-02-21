#include <stdio.h> 
#include <stdlib.h> 
 
int main() { 
    int numAllocations; 

    size_t allocationSize; 
    void **allocations; // Array to hold pointers to allocated memory 
    size_t totalMemoryAllocated = 0; 
 
    // User input for number of memory allocations 
    printf("Enter the number of memory allocations: "); 
    scanf("%d", &numAllocations); 
 
    // Allocate memory for storing the pointers to allocated blocks 
    allocations = (void **)malloc(numAllocations * sizeof(void *)); 

    if (allocations == NULL) { 
        fprintf(stderr, "Memory allocation for allocation pointers failed.\n"); 
        return 1; 
    } 
 
    // Loop to allocate memory 
    for (int i = 0; i < numAllocations; i++) { 
        printf("Enter the size of allocation %d: ", i + 1); 
        scanf("%zu", &allocationSize); 
 
        // Allocate memory 
        allocations[i] = malloc(allocationSize); 
        
        if (allocations[i] == NULL) { 
            fprintf(stderr, "Memory allocation failed for allocation %d.\n", i + 1); 
            // Free any previously allocated memory before exiting 
            for (int j = 0; j < i; j++) { 
                free(allocations[j]); 
            } 
            free(allocations); 
            return 1; 
        } 
 
        // Update total memory allocated 
        totalMemoryAllocated += allocationSize; 
        printf("Allocated %zu bytes in allocation %d.\n", allocationSize, i + 1); 
    } 
 
    // Display total memory allocated 
    printf("\nTotal memory allocated without freeing: %zu bytes\n", totalMemoryAllocated); 
 
    // Here, the program ends without freeing allocated memory, simulating a memory leak. 

 
    return 0; 
}