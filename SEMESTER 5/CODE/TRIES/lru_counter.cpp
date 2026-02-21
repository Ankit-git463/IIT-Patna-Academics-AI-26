#include <iostream>
#include <unordered_map>
#include <climits>

class LRUCounter {
private:
    std::unordered_map<int, int> pageTable; // page number -> last access time
    int clock;

public:
    LRUCounter() : clock(0) {}

    void referencePage(int page) {
        // Increment the clock and update the page's last access time
        clock++;
        pageTable[page] = clock;
        std::cout << "Page " << page << " referenced at time " << clock << std::endl;
    }

    int findLRUPage() {
        int lruPage = -1;
        int minCounter = INT_MAX;
        for (const auto &entry : pageTable) {
            if (entry.second < minCounter) {
                minCounter = entry.second;
                lruPage = entry.first;
            }
        }
        return lruPage;
    }

    void displayPages() {
        for (const auto &entry : pageTable) {
            std::cout << "Page: " << entry.first << ", Last Access Time: " << entry.second << std::endl;
        }
    }
};

int main() {
    LRUCounter lru;
    lru.referencePage(1);
    lru.referencePage(2);
    lru.referencePage(3);
    lru.referencePage(4);
    lru.referencePage(1); // Reference page 1 again, it moves to the top
    lru.referencePage(5);
    lru.referencePage(6); // Stack exceeds limit, oldest page removed

    std::cout << "LRU Page: " << lru.findLRUPage() << std::endl;
    lru.displayPages();

    return 0;
}
