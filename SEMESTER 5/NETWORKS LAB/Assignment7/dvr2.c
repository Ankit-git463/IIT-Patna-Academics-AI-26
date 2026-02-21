#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <limits.h>   // for INT_MAX
#include <string.h>   // for strcpy()

#define MAX_ROUTERS 10
#define INF INT_MAX  // Representing infinity for unreachable nodes
#define MAX_ITERATIONS 10  // Limit the number of iterations

// Structure to represent a Router's routing table
typedef struct {
    char destination;
    int cost;
    char nextHop;
} RoutingEntry;

// Structure for Router
typedef struct {
    char name;
    int id;
    int numNeighbors;
    RoutingEntry routingTable[MAX_ROUTERS];
    int neighbors[MAX_ROUTERS];  // Neighboring routers' IDs
    int costs[MAX_ROUTERS];      // Costs to neighbors
    HANDLE mutex;                // Mutex to protect routing table
} Router;

// Global Variables
Router routers[MAX_ROUTERS];
int numRouters = 0;
HANDLE routerThreads[MAX_ROUTERS];
int iterationCount = 0; // Track the iteration number

// Function to initialize the router's routing table
void initializeRoutingTable(Router *router) {
    WaitForSingleObject(router->mutex, INFINITE);

    for (int i = 0; i < numRouters; i++) {
        router->routingTable[i].destination = routers[i].name;
        if (i == router->id) {
            router->routingTable[i].cost = 0;
            router->routingTable[i].nextHop = router->name;
        } else {
            router->routingTable[i].cost = INF;
            router->routingTable[i].nextHop = '-';
        }
    }

    ReleaseMutex(router->mutex);
}

// Function to simulate sending and receiving routing table updates
DWORD WINAPI routerThreadFunction(LPVOID arg) {
    Router *router = (Router*)arg;

    while (iterationCount < MAX_ITERATIONS) {
        // Sleep for 3 seconds to simulate sending updates
        Sleep(3000);

        // Lock router's mutex
        WaitForSingleObject(router->mutex, INFINITE);

        // Send routing table to neighbors
        for (int i = 0; i < router->numNeighbors; i++) {
            int neighborID = router->neighbors[i];
            Router *neighbor = &routers[neighborID];

            // Lock the neighbor's mutex and update its routing table
            WaitForSingleObject(neighbor->mutex, INFINITE);

            for (int j = 0; j < numRouters; j++) {
                int newCost = router->routingTable[j].cost + router->costs[i];
                if (newCost < neighbor->routingTable[j].cost) {
                    neighbor->routingTable[j].cost = newCost;
                    neighbor->routingTable[j].nextHop = router->name;
                }
            }

            ReleaseMutex(neighbor->mutex);
        }

        ReleaseMutex(router->mutex);

        // Print routing table
        WaitForSingleObject(router->mutex, INFINITE);
        printf("\nIteration %d Router %c Routing Table:\n", iterationCount, router->name);
        printf("Destination   Cost   NextHop\n");
        for (int i = 0; i < numRouters; i++) {
            printf("%-13c %-7d %c\n",
                   router->routingTable[i].destination,
                   router->routingTable[i].cost == INF ? -1 : router->routingTable[i].cost,
                   router->routingTable[i].nextHop);
        }
        ReleaseMutex(router->mutex);

        // Increase iteration count after printing all routers' tables
        iterationCount++;
    }

    return 0;
}

// Function to parse the topology file and initialize routers
void parseTopologyFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open topology file");
        exit(EXIT_FAILURE);
    }

    fscanf(file, "%d", &numRouters);

    // Debug print to confirm the number of routers
    printf("Number of routers: %d\n", numRouters);

    // Read router names
    for (int i = 0; i < numRouters; i++) {
        fscanf(file, " %c", &routers[i].name);
        printf("Router initialized: %c\n", routers[i].name);  // Debug print for router names
        routers[i].id = i;
        routers[i].numNeighbors = 0;
        routers[i].mutex = CreateMutex(NULL, FALSE, NULL);
        if (routers[i].mutex == NULL) {
            fprintf(stderr, "Error creating mutex for router %c\n", routers[i].name);
            exit(EXIT_FAILURE);
        }
    }

    // Read link costs
    char router1, router2;
    int cost;
    while (fscanf(file, " %c %c %d", &router1, &router2, &cost) == 3) {
        int id1 = -1, id2 = -1;
        
        // Debug print for link cost
        printf("Read link: %c to %c with cost %d\n", router1, router2, cost);

        for (int i = 0 ; i < numRouters; i++) {
            if (routers[i].name == router1) id1 = i;
            if (routers[i].name == router2) id2 = i;
        }

        // Check if router names are valid
        if (id1 == -1 || id2 == -1) {
            fprintf(stderr, "Invalid router names: %c or %c in topology file.\n", router1, router2);
            fclose(file);
            exit(EXIT_FAILURE);
        }

        // Add neighbors
        routers[id1].neighbors[routers[id1].numNeighbors] = id2;
        routers[id1].costs[routers[id1].numNeighbors] = cost;
        routers[id1].numNeighbors++;

        routers[id2].neighbors[routers[id2].numNeighbors] = id1;
        routers[id2].costs[routers[id2].numNeighbors] = cost;
        routers[id2].numNeighbors++;
    }

    fclose(file);
}

int main() {
    // Parse the topology file
    parseTopologyFile("topology.txt");

    // Initialize each router's routing table
    for (int i = 0; i < numRouters; i++) {
        initializeRoutingTable(&routers[i]);
    }

    // Create a thread for each router
    for (int i = 0; i < numRouters; i++) {
        routerThreads[i] = CreateThread(NULL, 0, routerThreadFunction, &routers[i], 0, NULL);
    }

    // Wait for all router threads to finish
    WaitForMultipleObjects(numRouters, routerThreads, TRUE, INFINITE);

    return 0;
}
