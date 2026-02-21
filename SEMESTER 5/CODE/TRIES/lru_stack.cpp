#include <iostream>
#include <list>
#include <unordered_map>

class LRUStack {
private:
    std::list<int> pageStack;
    std::unordered_map<int, std::list<int>::iterator> pageMap;

public:
    void referencePage(int page) {
        // If the page is already in the stack, remove it from its current position
        if (pageMap.find(page) != pageMap.end()) {
            pageStack.erase(pageMap[page]);
        }
        // Insert the page at the top of the stack
        pageStack.push_front(page);
        pageMap[page] = pageStack.begin();

        // If the stack grows beyond a certain size, remove the least recently used page
        if (pageStack.size() > 5) { // Assume cache size is 5 for this example
            int lruPage = pageStack.back();
            pageStack.pop_back();
            pageMap.erase(lruPage);
        }

        std::cout << "Page " << page << " referenced, moved to top of stack." << std::endl;
    }

    void displayPages() {
        std::cout << "Current page stack (Most recent -> Least recent): ";
        for (const int &page : pageStack) {
            std::cout << page << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    LRUStack lru;
    lru.referencePage(1);
    lru.referencePage(2);
    lru.referencePage(3);
    lru.referencePage(4);
    lru.referencePage(1); // Reference page 1 again, it moves to the top
    lru.referencePage(5);
    lru.referencePage(6); // Stack exceeds limit, oldest page removed

    lru.displayPages();

    return 0;
}
