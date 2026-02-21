#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <limits>
#include <string>
#include <unordered_map>
#include <fstream> // Include for file handling

using namespace std;

int MAX_ITERATIONS = 10;
const int INF = numeric_limits<int>::max(); // Use max integer for infinity

struct Router {
    string name;
    vector<int> costs;      // Cost to reach each router
    vector<string> nextHops; // Next hop to reach each router
};

// Global variables
vector<Router> routers;
int iterationCount = 0;
mutex routingTableMutex;

// Function to print the routing table of a router
void printRoutingTable(const Router &router) {
    cout << "Iteration " << iterationCount << " - Router " << router.name << " Routing Table:\n";
    cout << "-----------------------------------------\n";
    cout << "| Destination | Cost | Next Hop |\n";
    cout << "-----------------------------------------\n";
    for (size_t i = 0; i < routers.size(); i++) {
        cout << "| " << setw(12) << routers[i].name << " | " 
             << setw(4) << (router.costs[i] == INF ? "INF" : to_string(router.costs[i])) << " | "
             << setw(8) << (router.nextHops[i].empty() ? "-" : router.nextHops[i]) << " |\n";
    }
    cout << "-----------------------------------------\n";
}

// Function to update the routing table of a router
void updateRoutingTable(Router &router) {
    vector<int> newCosts = router.costs;
    vector<string> newNextHops = router.nextHops;

    for (size_t i = 0; i < routers.size(); i++) {
        if (i == &router - &routers[0]) continue; // Skip self

        // Check if the cost to reach i is not INF (meaning a connection exists)
        if (router.costs[i] != INF) {
            for (size_t j = 0; j < routers.size(); j++) {
                if (j == &router - &routers[0]) continue; // Skip self
                
                // Calculate the new cost
                if (router.costs[i] != INF && routers[i].costs[j] != INF) {
                    long long newCost = static_cast<long long>(router.costs[i]) + routers[i].costs[j];

                    // Check for overflow and update if the new cost is lower
                    if (newCost < INF && newCost < newCosts[j]) {
                        newCosts[j] = static_cast<int>(newCost);
                        newNextHops[j] = router.name;
                    }
                }
            }
        }
    }

    router.costs = newCosts;
    router.nextHops = newNextHops;
}

// Thread function for each router
void routerThread(Router &router) {
    while (iterationCount < MAX_ITERATIONS) {
        updateRoutingTable(router);

        {
            lock_guard<mutex> lock(routingTableMutex);
            printRoutingTable(router);

            cout << "\nAll Routers Routing Tables after Iteration " << iterationCount << ":\n";
            for (const auto &r : routers) {
                printRoutingTable(r);
            }
            cout << endl;
        }

        this_thread::sleep_for(chrono::seconds(2));
        iterationCount++;
    }
}

void readInputFromFile(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open file: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    int numberOfRouters;
    file >> numberOfRouters;
    routers.resize(numberOfRouters);
    MAX_ITERATIONS = numberOfRouters;

    // Input router names
    for (int i = 0; i < numberOfRouters; i++) {
        file >> routers[i].name;
        routers[i].costs.resize(numberOfRouters, INF);
        routers[i].nextHops.resize(numberOfRouters, "");
        routers[i].costs[i] = 0; // Cost to self is 0
        routers[i].nextHops[i] = routers[i].name; // Next hop to self
    }

    int numberOfLinks;
    file >> numberOfLinks;

    // Input links and costs
    for (int i = 0; i < numberOfLinks; i++) {
        string router1, router2;
        int cost;
        file >> router1 >> router2 >> cost;

        // Find indices of routers
        int index1 = -1, index2 = -1;
        for (int j = 0; j < numberOfRouters; j++) {
            if (routers[j].name == router1) index1 = j;
            if (routers[j].name == router2) index2 = j;
        }

        // Update costs
        if (index1 != -1 && index2 != -1) {
            routers[index1].costs[index2] = cost;
            routers[index2].costs[index1] = cost; // Assuming bidirectional links
        }
    }

    file.close();
}

int main() {
    string filename;
    cout << "Enter the filename for input: ";
    cin >> filename; // Get filename from user
    readInputFromFile(filename); // Read input from the file

    vector<thread> threads;

    // Create threads for each router
    for (auto &router : routers) {
        threads.push_back(thread(routerThread, ref(router)));
    }

    // Join threads
    for (auto &t : threads) {
        t.join();
    }
    
    
    cout<<"------------------- DONE -----------------"<<endl;
    return 0;
}