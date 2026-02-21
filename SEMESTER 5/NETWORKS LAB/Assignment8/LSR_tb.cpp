#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <thread>
#include <mutex>
#include <limits>
#include <atomic>
#include <iomanip>

using namespace std;

struct Router {
    string name;
    map<string, int> routingTable; // Map to store shortest path cost to each router
    map<string, int> neighbors;    // Direct neighbors and their link costs
    set<string> allRouters;        // Set of all router names for Dijkstra
};

map<string, Router> routers;
mutex displayMutex; // Mutex for synchronized display
atomic<int> activeThreads{0}; // Counter to synchronize threads during initialization

// Function to parse the topology file
bool parseTopology(string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open topology file!" << endl;
        return false;
    }
    
    int routerCount;
    file >> routerCount;
    
    vector<string> routerNames(routerCount);
    for (int i = 0; i < routerCount; ++i) {
        file >> routerNames[i];
        routers[routerNames[i]].name = routerNames[i];
    }

    string src, dest;
    int cost;
    while (file >> src && src != "END") {
        file >> dest >> cost;
        routers[src].neighbors[dest] = cost;
        routers[dest].neighbors[src] = cost;
        routers[src].allRouters.insert(dest);
        routers[dest].allRouters.insert(src);
    }

    for (auto& router : routers) {
        for (const auto& name : routerNames) {
            router.second.allRouters.insert(name);
            router.second.routingTable[name] = (name == router.first) ? 0 : numeric_limits<int>::max();
        }
    }
    return true;
}

// Dijkstra's Algorithm
void dijkstra(string routerName) {
    Router& router = routers[routerName];
    map<string, int>& distances = router.routingTable;
    set<string> visited;
    
    while (visited.size() < router.allRouters.size()) {
        string closest;
        int minDist = numeric_limits<int>::max();

        // Find the closest unvisited router
        for (const auto& [node, dist] : distances) {
            if (visited.find(node) == visited.end() && dist < minDist) {
                minDist = dist;
                closest = node;
            }
        }

        if (closest.empty()) break;
        visited.insert(closest);

        // Update distances to neighbors of the closest node
        for (const auto& [neighbor, linkCost] : routers[closest].neighbors) {
            if (distances[neighbor] > distances[closest] + linkCost) {
                distances[neighbor] = distances[closest] + linkCost;
            }
        }
    }

    // Display the routing table for this router in a tabular format
    lock_guard<mutex> lock(displayMutex);
    cout << "Routing Table for Router " << routerName << " (Converged):\n";
    cout << "+-------------+----------+\n";
    cout << "| Destination |   Cost   |\n";
    cout << "+-------------+----------+\n";
    for (const auto& [dest, cost] : distances) {
        cout << "| " << setw(11) << left << dest 
             << " | " << setw(8) << (cost == numeric_limits<int>::max() ? "INF" : to_string(cost)) << " |\n";
    }
    cout << "+-------------+----------+\n\n";
}

// Thread function for each router
void routerThreadFunction(string routerName) {
    // Run Dijkstra's algorithm for shortest path calculation
    dijkstra(routerName);

    // Signal convergence and terminate the router thread
    cout << "Network has converged. Terminating router " << routerName << ".\n";
}

int main() {
    // Step 1: Parse the topology
    if (!parseTopology("topology.txt")) return 1;

    // Step 2: Start router threads to compute shortest paths
    vector<thread> threads;
    for (const auto& router : routers) {
        threads.emplace_back(routerThreadFunction, router.first);
    }

    // Join threads
    for (auto& th : threads) {
        th.join();
    }

    return 0;
}
