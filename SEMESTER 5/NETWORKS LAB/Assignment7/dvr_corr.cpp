#include <iostream>
#include <fstream>
#include <limits>
#include <vector>
#include <string>

#define INF std::numeric_limits<int>::max()  // Representing infinity for unreachable nodes
#define MAX_ITERATIONS 10  // Limit the number of iterations

// Structure to represent a Router's routing table
struct RoutingEntry {
    char destination;
    int cost;
    char nextHop;
};

// Structure for Router
struct Router {
    char name;
    int id;
    std::vector<RoutingEntry> routingTable;
    std::vector<int> neighbors;  // Neighboring routers' IDs
    std::vector<int> costs;      // Costs to neighbors
};

// Global Variables
std::vector<Router> routers;
int numRouters = 0;
int iterationCount = 0; // Track the iteration number
int links = 0;

// Function to initialize the router's routing table
void initializeRoutingTable(Router &router) {
    for (int i = 0; i < numRouters; i++) {
        RoutingEntry entry;
        entry.destination = routers[i].name;
        if (i == router.id) {
            entry.cost = 0;
            entry.nextHop = router.name;
        } else {
            entry.cost = INF;
            entry.nextHop = '-';
        }
        router.routingTable.push_back(entry);
    }
}

// Function to simulate sending and receiving routing table updates
void updateRoutingTable(Router &router) {
    // Send routing table to neighbors
    for (size_t i = 0; i < router.neighbors.size(); i++) {
        int neighborID = router.neighbors[i];
        Router &neighbor = routers[neighborID];

        // Update neighbor's routing table
        for (size_t j = 0; j < numRouters; j++) {
            int newCost = router.routingTable[j].cost + router.costs[i];
            if (newCost < neighbor.routingTable[j].cost) {
                neighbor.routingTable[j].cost = newCost;
                neighbor.routingTable[j].nextHop = router.name;
            }
        }
    }
}

// Function to print the routing table of a router
// void printRoutingTable(const Router &router) {
//     std::cout << "\nIteration " << iterationCount << " Router " << router.name << " Routing Table:\n";
//     std::cout << "Destination   Cost   NextHop\n";
//     for (const auto &entry : router.routingTable) {
//         std::cout << entry.destination << "            "
//                   << (entry.cost == INF ? -1 : entry.cost) << "       "
//                   << entry.nextHop << "\n";
//     }
// }

void printRoutingTable(const Router &router) {
    std::cout << "\nIteration " << iterationCount << " Router " << router.name << " Routing Table:\n";
    std::cout << "Destination   Cost   NextHop\n";
    for (const auto &entry : router.routingTable) {
        std::cout << entry.destination << "            ";
        
        // Check if the cost is INF, if so print "INF", otherwise print the cost value
        if (entry.cost == INF) {
            std::cout << "INF";
        } else {
            std::cout << entry.cost;
        }
        
        std::cout << "       " << entry.nextHop << "\n";
    }
}


// Function to simulate the DVR algorithm
void simulateDVR() {
    while (iterationCount < MAX_ITERATIONS) {
        // Update routing tables for all routers
        for (int i = 0; i < numRouters; i++) {
            updateRoutingTable(routers[i]);
        }

        // Print routing tables for all routers
        for (int i = 0; i < numRouters; i++) {
            printRoutingTable(routers[i]);
        }

        // Increase iteration count
        iterationCount++;
    }
}

// Function to parse the topology file and initialize routers
void parseTopologyFile(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open topology file\n";
        exit(EXIT_FAILURE);
    }

    file >> numRouters;
    std::cout << "Number of routers: " << numRouters << "\n";

    // Read router names
    routers.resize(numRouters);
    for (int i = 0; i < numRouters; i++) {
        file >> routers[i].name;
        std::cout << "Router initialized: " << routers[i].name << "\n";
        routers[i].id = i;
    }

    file >> links;
    std::cout << "Number of links: " << links << "\n";

    int count = 0;
    // Read link costs
    char router1, router2;
    int cost;
    while (count < links) {
        count++;
        file >> router1 >> router2 >> cost;
        std::cout << "Read link: " << router1 << " to " << router2 << " with cost " << cost << "\n";

        int id1 = -1, id2 = -1;
        for (int i = 0; i < numRouters; i++) {
            if (routers[i].name == router1) id1 = i;
            if (routers[i].name == router2) id2 = i;
        }

        if (id1 == -1 || id2 == -1) {
            std::cerr << "Invalid router names: " << router1 << " or " << router2 << " in topology file.\n";
            file.close();
            exit(EXIT_FAILURE);
        }

        routers[id1].neighbors.push_back(id2);
        routers[id1].costs.push_back(cost);

        routers[id2].neighbors.push_back(id1);
        routers[id2].costs.push_back(cost);
    }

    file.close();
}

int main() {
    // Parse the topology file
    parseTopologyFile("topo.txt");

    // Initialize each router's routing table
    for (int i = 0; i < numRouters; i++) {
        initializeRoutingTable(routers[i]);
    }

    // Simulate DVR algorithm
    simulateDVR();

    return 0;
}
