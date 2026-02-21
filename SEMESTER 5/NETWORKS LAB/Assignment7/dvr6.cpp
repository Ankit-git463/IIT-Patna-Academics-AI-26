#include <stdio.h>
#include <stdlib.h>
#include <limits.h>   // for INT_MAX
#include <string.h>   // for strcpy()
#include <map>
#include <vector>

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
} Router;

// Global Variables
Router routers[MAX_ROUTERS];
int numRouters = 0;
int iterationCount = 0; // Track the iteration number
int links = 0 ; 

// Function to initialize the router's routing table
void initializeRoutingTable(Router *router) {
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
}

// Function to simulate sending and receiving routing table updates
void updateRoutingTable(Router *router) {
    // Send routing table to neighbors
    for (int i = 0; i < router->numNeighbors; i++) {
        int neighborID = router->neighbors[i];
        Router *neighbor = &routers[neighborID];

        // Update neighbor's routing table
        for (int j = 0; j < numRouters; j++) {
            int newCost = router->routingTable[j].cost + router->costs[i];
            if (newCost < neighbor->routingTable[j].cost) {
                neighbor->routingTable[j].cost = newCost;
                neighbor->routingTable[j].nextHop = router->name;
            }
        }
    }
}

// Function to print the routing table of a router
void printRoutingTable(Router *router) {
    printf("\nIteration %d Router %c Routing Table:\n", iterationCount, router->name);
    printf("Destination   Cost   NextHop\n");
    for (int i = 0; i < numRouters; i++) {
        printf("%-13c %-7d %c\n",
               router->routingTable[i].destination,
               router->routingTable[i].cost == INF ? -1 : router->routingTable[i].cost,
               router->routingTable[i].nextHop);
    }
}

// Function to simulate the DVR algorithm
void simulateDVR() {
    while (iterationCount < MAX_ITERATIONS) {
        // Update routing tables for all routers
        for (int i = 0; i < numRouters; i++) {
            updateRoutingTable(&routers[i]);
        }

        // Print routing tables for all routers
        for (int i = 0; i < numRouters; i++) {
            printRoutingTable(&routers[i]);
        }

        // Increase iteration count
        iterationCount++;
    }
}

// Function to parse the topology file and initialize routers
void parseTopologyFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open topology file");
        exit(EXIT_FAILURE);
    }

    fscanf(file, "%d", &numRouters);
    printf("Number of routers: %d\n", numRouters);

    // Read router names
    for (int i = 0; i < numRouters; i++) {
        fscanf(file, " %c", &routers[i].name);
        printf("Router initialized: %c\n", routers[i].name);
        routers[i].id = i;
        routers[i].numNeighbors = 0;
    }

    fscanf(file, "%d", &links);
    printf("Number of routers: %d\n", links);

    int count= 0; 

    // Read link costs
    char router1, router2;
    int cost;
    while (count < links) {
        count++ ; 
        fscanf(file, " %c %c %d", &router1, &router2, &cost);
        int id1 = -1, id2 = -1;
        printf("Read link: %c to %c with cost %d\n", router1, router2, cost);

        for (int i = 0; i < numRouters; i++) {
            if (routers[i].name == router1) id1 = i;
            if (routers[i].name == router2) id2 = i;
        }

        if (id1 == -1 || id2 == -1) {
            fprintf(stderr, "Invalid router names: %c or %c in topology file.\n", router1, router2);
            fclose(file);
            exit(EXIT_FAILURE);
        }

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
    parseTopologyFile("topo.txt");

    // Initialize each router's routing table
    for (int i = 0; i < numRouters; i++) {
        initializeRoutingTable(&routers[i]);
    }

    // Simulate DVR algorithm
    simulateDVR();

    return 0;
}
