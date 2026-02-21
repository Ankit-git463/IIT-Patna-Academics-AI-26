#include <bits/stdc++.h>
#include <iostream>
#include <limits>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;
#define INF numeric_limits<int>::max()  // Representing infinity for unreachable nodes
#define MAX_ITERATIONS 10  // Limit the number of iterations

// Mutex for protecting access to routing tables
mutex routingTableMutex;

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
    vector<RoutingEntry> routingTable;
    vector<int> neighbors;  // Neighboring routers' IDs
    vector<int> costs;      // Costs to neighbors
};

// Global Variables
vector<Router> routers;
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
    // Lock the routing table to prevent concurrent modification
    lock_guard<mutex> lock(routingTableMutex);

    // Send routing table to neighbors
    for (size_t i = 0; i < router.neighbors.size(); i++) {
        int neighborID = router.neighbors[i];
        Router &neighbor = routers[neighborID];

        // Update neighbor's routing table
        for (size_t j = 0; j < numRouters; j++) {
            // Only proceed if the current cost is not INF
            if (router.routingTable[j].cost < INF) {
                int newCost = router.routingTable[j].cost + router.costs[i];

                // Debug output to check the values
                cout << "Router " << router.name << " to " << neighbor.name 
                     << ": current cost = " << router.routingTable[j].cost 
                     << ", link cost = " << router.costs[i] 
                     << ", new cost = " << newCost << endl;

                if (newCost < neighbor.routingTable[j].cost) {
                    neighbor.routingTable[j].cost = newCost;
                    neighbor.routingTable[j].nextHop = router.name;
                }
            }
        }
    }
}

// Function to print the routing table of a router
void printRoutingTable(const Router &router) {
    // Lock the routing table for safe access
    lock_guard<mutex> lock(routingTableMutex);

    cout << "\nIteration " << iterationCount << " - Router " << router.name << " Routing Table:\n";
    cout << "-----------------------------------------\n";
    cout << "| Destination | Cost | Next Hop |\n";
    cout << "-----------------------------------------\n";
    for (const auto &entry : router.routingTable) {
        cout << "| " << setw(12) << entry.destination 
             << " | " << setw(4) << (entry.cost == INF ? "INF" : to_string(entry.cost))
             << " | " << setw(9) << entry.nextHop << " |\n";
    }
    cout << "-----------------------------------------\n";
}

// Function for a router to perform its updates and print its routing table in a separate thread
void routerThread(Router &router) {
    while (iterationCount < MAX_ITERATIONS) {
        // Update routing table
        updateRoutingTable(router);

        // Print routing table
        printRoutingTable(router);

        // Sleep for 2 seconds to simulate asynchronous updates
        this_thread::sleep_for(chrono::seconds(2));

        // Increment the iteration count
        iterationCount++;
    }
}

// Function to simulate the DVR algorithm with multithreading
void simulateDVR() {
    // Create a thread for each router
    vector<thread> threads;

    for (int i = 0; i < numRouters; i++) {
        threads.push_back(thread(routerThread, ref(routers[i])));
    }

    // Join the threads (wait for them to finish)
    for (auto &t : threads) {
        t.join();
    }
}

// Function to collect input from the user for routers and links
void collectUserInput() {
    cout << "Enter the number of routers: ";
    cin >> numRouters;

    // Resize the routers vector
    routers.resize(numRouters);

    // Read router names
    for (int i = 0; i < numRouters; i++) {
        cout << "Enter the name of router " << (i + 1) << ": ";
        cin >> routers[i].name;
        routers[i].id = i;
    }

    cout << "Enter the number of links: ";
    cin >> links;

    cout << "Enter the links in the format: <router1> <router2> <cost>\n";
    cout << "(Type 'done' to finish entering links)\n";

    int count = 0;
    while (count < links) {
        char router1, router2;
        int cost;

        // Get user input for the link
        cout << "Link " << (count + 1) << ": ";
        cin >> router1 >> router2;

        if (router1 == 'd' && router2 == 'o') { // simple check for 'done'
            break;
        }

        cin >> cost;

        cout << "Read link: " << router1 << " to " << router2 << " with cost " << cost << "\n";

        int id1 = -1, id2 = -1;
        for (int i = 0; i < numRouters; i++) {
            if (routers[i].name == router1) id1 = i;
            if (routers[i].name == router2) id2 = i;
        }

        if (id1 == -1 || id2 == -1) {
            cerr << "Invalid router names: " << router1 << " or " << router2 << ". Please try again.\n";
            continue; // Skip to next iteration if the input is invalid
        }

        // Add the link to both routers
        routers[id1].neighbors.push_back(id2);
        routers[id1].costs.push_back(cost);
        routers[id2].neighbors.push_back(id1);
        routers[id2].costs.push_back(cost);
        count++;
    }
}

int main() {
    // Collect user input
    collectUserInput();

    // Initialize each router's routing table
    for (int i = 0; i < numRouters; i++) {
        initializeRoutingTable(routers[i]);
    }

    // Simulate DVR algorithm with thread safety
    simulateDVR();

    return 0;
}
