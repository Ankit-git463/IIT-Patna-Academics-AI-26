#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <climits> // For INF
#include <queue>
#include <thread>
#include <chrono> // For sleep
#include <mutex>

using namespace std;

const int INF = INT_MAX;  // Infinity value for unreachable routes

struct Link {
    char src;
    char dest;
    int cost;
};

struct RoutingTable {
    map<char, int> distances;  // Distance to each router
    map<char, char> nextHop;   // Next hop for each destination
};

vector<char> routers;                  // List of routers
vector<Link> links;                    // List of links (edges)
map<char, RoutingTable> routingTables; // Routing tables for each router
mutex mtx;                             // Mutex for thread-safe table updates

// Function to parse the topology.txt file
void parseTopology(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Unable to open topology file.\n";
        exit(1);
    }

    string line;
    
    // First line: number of routers (we ignore it)
    getline(file, line);
    int numRouters = stoi(line);

    // Second line: router names
    getline(file, line);
    stringstream ss(line);
    char router;
    while (ss >> router) {
        routers.push_back(router);
    }

    // Read links until "END"
    while (getline(file, line) && line != "END") {
        stringstream ss(line);
        char src, dest;
        int cost;
        ss >> src >> dest >> cost;
        links.push_back({src, dest, cost});
    }

    file.close();
}

// Function to check if the graph is connected using BFS
bool isConnected() {
    map<char, vector<char>> adjList;
    
    // Build adjacency list
    for (const auto& link : links) {
        adjList[link.src].push_back(link.dest);
        adjList[link.dest].push_back(link.src);  // Since it's undirected
    }

    // Perform BFS to check connectivity
    map<char, bool> visited;
    queue<char> q;
    q.push(routers[0]);  // Start with the first router
    visited[routers[0]] = true;

    while (!q.empty()) {
        char curr = q.front();
        q.pop();

        for (char neighbor : adjList[curr]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }

    // Check if all routers were visited
    for (char r : routers) {
        if (!visited[r]) {
            return false;  // Graph is disconnected
        }
    }

    return true;  // Graph is connected
}

// Function to initialize routing tables for each router
void initializeRoutingTables() {
    for (char router : routers) {
        RoutingTable table;
        for (char dest : routers) {
            table.distances[dest] = (router == dest) ? 0 : INF;
            table.nextHop[dest] = (router == dest) ? router : '-';
        }

        // Set direct neighbors' costs
        for (const auto& link : links) {
            if (link.src == router) {
                table.distances[link.dest] = link.cost;
                table.nextHop[link.dest] = link.dest;
            } else if (link.dest == router) {
                table.distances[link.src] = link.cost;
                table.nextHop[link.src] = link.src;
            }
        }

        routingTables[router] = table;
    }
}

// Function for Bellman-Ford update on a router's table
bool bellmanFordUpdate(char router, const RoutingTable& neighborTable, char neighbor) {
    bool updated = false;

    for (const auto& entry : neighborTable.distances) {
        char destination = entry.first;
        int newCost = (neighborTable.distances[destination] == INF) ? INF : 
                       neighborTable.distances[destination] + routingTables[router].distances[neighbor];

        // Update the router's table if a shorter path is found
        if (routingTables[router].distances[destination] > newCost) {
            routingTables[router].distances[destination] = newCost;
            routingTables[router].nextHop[destination] = neighbor;
            updated = true;
        }
    }

    return updated;
}

// Thread function for each router
void routerThread(char router) {
    int iteration = 0;
    bool updated = true;

    while (updated) {
        mtx.lock();  // Lock for thread-safe output
        cout << "Iteration " << iteration++ << " for Router " << router << "\n";
        cout << "Routing Table for Router " << router << ":\n";
        
        // Display routing table
        for (const auto& entry : routingTables[router].distances) {
            cout << "Destination: " << entry.first 
                 << " | Cost: " << (entry.second == INF ? "INF" : to_string(entry.second)) 
                 << " | Next Hop: " << routingTables[router].nextHop[entry.first] << "\n";
        }
        mtx.unlock();

        // Simulate forwarding table to neighbors (sleep for 3 seconds)
        this_thread::sleep_for(chrono::seconds(3));

        // Update tables using Bellman-Ford
        updated = false;  // Reset update flag
        for (const auto& link : links) {
            if (link.src == router || link.dest == router) {
                char neighbor = (link.src == router) ? link.dest : link.src;

                mtx.lock();
                bool flag = bellmanFordUpdate(router, routingTables[neighbor], neighbor);
                mtx.unlock();
                
                if (flag) {
                    updated = true;  // Set update flag if table changes
                }
            }
        }

        // Wait for 2 seconds before the next iteration
        this_thread::sleep_for(chrono::seconds(2));
    }
}

int main() {
    parseTopology("topology.txt");

    if (!isConnected()) {
        cout << "Dis-connected graph\n";
        return 1;
    }

    initializeRoutingTables();

    // Create a thread for each router
    vector<thread> routerThreads;
    for (char router : routers) {
        routerThreads.push_back(thread(routerThread, router));
    }

    // Join all threads
    for (auto& t : routerThreads) {
        t.join();
    }

    return 0;
}
