#include <stdio.h> 
#include <stdlib.h> 
 
int findMissingPage(int arr[], int size) { 

    // Iterate through the array to place each page number in its correct index 
    
    for (int i = 0; i < size; i++) { 
        // Continue swapping until the current element is in its right position 
        
        while (arr[i] >= 0 && arr[i] < size && arr[i] != arr[arr[i]]) { 
            int temp = arr[i]; 
            arr[i] = arr[temp]; 
            arr[temp] = temp; 
        } 
    } 
 
    // After rearranging, the first index not matching the value is the missing page 
    for (int i = 0; i < size; i++) { 
        if (arr[i] != i) { 
            return i; // First missing page 
        }

            } 
     
    // If all pages are present, the next missing page is size 
    return size; 
} 
 
int main() { 
    int size; 
 
    // Prompt user for input 
    printf("Enter the number of pages: "); 
    scanf("%d", &size); 
 
    int *pages = (int *)malloc(size * sizeof(int)); // Dynamically allocate memory for pages 
 
    printf("Enter the page numbers:\n"); 
    
    for (int i = 0; i < size; i++) { 
        scanf("%d", &pages[i]); // User input for each page number 
    } 
 
    int missingPage = findMissingPage(pages, size); 
    printf("The first missing page number is: %d\n", missingPage); 
 
    free(pages); // Free allocated memory 
    return 0; 
} 