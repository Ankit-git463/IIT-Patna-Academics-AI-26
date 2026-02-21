#include <stdio.h>
#include <stdlib.h>

#define N 10


// Adjacency matrix for the graph
int adjMatrix[2*N][2*N];
int visited[2*N];
int recStack[2* N];

int parent[2*N];
int numProcesses, numResources;
int cycleStart = -1, cycleEnd = -1;

// Function to add an edge in the adjacency matrix
void addEdge(int from, int to) {
    adjMatrix[from][to] = 1;
}

// Function to check for a cycle using DFS
int isCyclicUtil(int node) {
    
    visited[node] = 1;
    recStack[node] = 1;

    for (int i = 0; i < numProcesses + numResources; i++) {
        if (adjMatrix[node][i]) {
            
            if (!visited[i]) {
                parent[i] = node;
                if (isCyclicUtil(i))
                    return 1;
            } 
            
            else if (recStack[i]) {
                cycleStart = i;
                cycleEnd = node;
                return 1;
            }
        }
    }
    recStack[node] = 0;
    return 0;
}

// Function to check for deadlock and print the cycle
int isDeadlock() {

    for (int i = 0; i < numProcesses + numResources; i++) {
        if (!visited[i] && isCyclicUtil(i)) {
            return 1;
        }
    }
    return 0;
}

// Function to print the deadlock cycle
void printCycle() {
    printf("Deadlock detected: ");
    int temp = cycleEnd;
    printf("Process %d -> ", cycleStart + 1);
    // Print the cycle in reverse order starting from cycleEnd to cycleStart
    while (temp != cycleStart) {
        if (temp < numProcesses) {
            printf("Process %d -> ", temp + 1);
        } else {
            printf("Resource %d -> ", temp - numProcesses + 1);
        }
        temp = parent[temp];
    }
    // Print the cycleStart to complete the cycle
    printf("Process %d\n", cycleStart + 1);
}

int main() {


    printf("Enter number of processes: ");
    scanf("%d", &numProcesses);
    printf("Enter number of resources: ");
    scanf("%d", &numResources);

    // Initialize adjacency matrix, visited array, recStack, and parent array
    
    for (int i = 0; i < numProcesses + numResources; i++) {
        
        for (int j = 0; j < numProcesses + numResources; j++) {
            adjMatrix[i][j] = 0;
        }

        visited[i] = 0;
        recStack[i] = 0;
        parent[i] = -1;
    }

    printf("Enter resource allocation (1 if process holds the resource, 0 otherwise):\n");

    for (int i = 0; i < numProcesses; i++) {
        printf("Process %d: ", i + 1);

        for (int j = 0; j < numResources; j++) {
            int allocation;

            scanf("%d", &allocation);
            if (allocation == 1) {
                // Add edge from resource node to process node
                addEdge(i, numProcesses + j);
            }
        }
    }

    printf("Enter resource request (1 if process is requesting the resource, 0 otherwise):\n");
    
    for (int i = 0; i < numProcesses; i++) {
        printf("Process %d: ", i + 1);
        for (int j = 0; j < numResources; j++) {
            int request;
            scanf("%d", &request);
            if (request == 1) {
                // Add edge from process node to resource node
                
                addEdge(numProcesses + j, i);
            }
        }
    }

    // Check for deadlock
    if (isDeadlock()) {
        printf("\n");
        printCycle();
        printf("\n");
    } else {
        printf("\n");
        printf("The system is deadlock-free.\n");
        printf("\n");
    }

    return 0;
}