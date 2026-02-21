#include <stdio.h> 
#include <stdlib.h> 
#define MAX_BLOCKS 100 


// Function to calculate total external fragmentation 
int calculateFragmentation(int allocated[], int deallocated[], int allocatedCount, int deallocatedCount) { 
    int totalAllocated = 0;
    int totalDeallocated = 0; 
    
    // Calculate total allocated memory 
    for (int i = 0; i < allocatedCount; i++) { 
        totalAllocated += allocated[i]; 
    } 
 
    // Calculate total deallocated memory 
    for (int i = 0; i < deallocatedCount; i++) { 
        totalDeallocated += deallocated[i]; 
    } 
 
    // Calculate the total external fragmentation 
    int totalExternalFragmentation = totalAllocated - totalDeallocated; 
 
    // Return the total external fragmentation, ensuring it doesn't go negative 
    return (totalExternalFragmentation < 0) ? 0 : totalExternalFragmentation; 
} 
 
int main() { 
    int allocated[MAX_BLOCKS], deallocated[MAX_BLOCKS]; 
    int allocatedCount, deallocatedCount; 
 
    // User input for the number of allocated memory blocks 
    printf("Enter the number of allocated memory blocks: "); 
    scanf("%d", &allocatedCount); 
 
    // User input for the sizes of allocated memory blocks 
    printf("Enter the sizes of allocated memory blocks (space-separated): "); 
    for (int i = 0; i < allocatedCount; i++) { 
        scanf("%d", &allocated[i]); 
    } 
 
    // User input for the number of deallocated memory blocks 
    printf("Enter the number of deallocated memory blocks: "); 
    scanf("%d", &deallocatedCount); 
 
    // User input for the sizes of deallocated memory blocks 
    printf("Enter the sizes of deallocated memory blocks (space-separated): "); 
    for (int i = 0; i < deallocatedCount; i++) { 
        scanf("%d", &deallocated[i]); 
    } 
 
    // Calculate total external fragmentation 
    int fragmentation = calculateFragmentation(allocated, deallocated, allocatedCount, deallocatedCount); 
 
    // Output the total external fragmentation 
    printf("Total external fragmentation after deallocations: %d\n", fragmentation); 
    return 0; 
} 