#include <iostream>
#include <vector>
#include <stack>
#include <set>

using namespace std;

const int ROWS = 16;
const int COLS = 16;

vector<vector<char>> maze = {
    {'S', 'W', 'W', 'W', ' ', ' ', ' ', ' ', 'W', ' ', ' ', 'W', ' ', ' ', ' ', 'E'},
    {' ', ' ', ' ', 'W', 'W', 'W', 'W', ' ', 'W', 'W', 'W', ' ', 'W', ' ', 'W', ' '},
    {' ', ' ', 'W', ' ', ' ', ' ', 'W', ' ', 'W', ' ', ' ', ' ', 'W', ' ', ' ', ' '},
    {' ', 'W', ' ', ' ', 'W', ' ', 'W', ' ', ' ', ' ', 'W', ' ', 'W', 'W', 'W', ' '},
    {' ', 'W', 'W', ' ', 'W', ' ', 'W', 'W', 'W', ' ', 'W', ' ', 'W', ' ', ' ', ' '},
    {' ', ' ', 'W', ' ', 'W', ' ', ' ', ' ', ' ', ' ', 'W', ' ', ' ', ' ', 'W', ' '},
    {' ', 'W', 'W', ' ', 'W', ' ', 'W', ' ', 'W', ' ', 'W', ' ', 'W', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', 'W', ' ', 'W', ' ', ' ', ' ', ' ', ' ', 'W', ' ', 'W', ' '},
    {'W', 'W', 'W', 'W', 'W', ' ', 'W', ' ', 'W', 'W', 'W', 'W', 'W', ' ', 'W', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', 'W', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'W', ' '},
    {'W', 'W', 'W', 'W', 'W', ' ', 'W', ' ', 'W', ' ', 'W', 'W', 'W', ' ', 'W', ' '},
    {' ', ' ', ' ', ' ', 'W', ' ', ' ', ' ', 'W', ' ', ' ', ' ', ' ', ' ', 'W', ' '},
    {' ', 'W', 'W', ' ', 'W', ' ', 'W', ' ', 'W', ' ', 'W', ' ', 'W', 'W', 'W', ' '},
    {' ', 'W', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'W', ' ', ' ', ' ', ' ', ' '},
    {' ', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', ' ', 'W', 'W', 'W', ' ', 'W', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'},
};

void printMaze(const vector<vector<char>> &maze) {
    for (const auto &row : maze) {
        for (char cell : row) {
            cout << cell << ' ';
        }
        cout << endl;
    }
}

vector<pair<int, int>> dfs(const vector<vector<char>> &maze, pair<int, int> start, pair<int, int> end) {
    stack<pair<pair<int, int>, vector<pair<int, int>>>> s;
    set<pair<int, int>> visited;

    vector<pair<int, int>> shortestPath;

    s.push({start, {}});

    while (!s.empty()) {
        auto current = s.top().first;
        auto path = s.top().second;
        s.pop();

        int x = current.first;
        int y = current.second;

        if (current == end) {
            if (shortestPath.empty() || path.size() < shortestPath.size()) {
                shortestPath = path;
            }
            continue;
        }

        for (const auto &dir : vector<pair<int, int>>{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}) {
            int dx = dir.first;
            int dy = dir.second;
            pair<int, int> newPos = {x + dx, y + dy};

            if (0 <= x + dx && x + dx < ROWS && 0 <= y + dy && y + dy < COLS &&
                maze[x + dx][y + dy] != 'W' && visited.find(newPos) == visited.end()) {
                s.push({newPos, path});
                s.top().second.push_back(current);
                visited.insert(newPos);
            }
        }
    }

    return shortestPath;
}

int main() {
    pair<int, int> start = {0, 0};
    pair<int, int> end = {0, 15};

    vector<pair<int, int>> dfsPath = dfs(maze, start, end);

    cout << "DFS Path:" << endl;
    for (auto point : dfsPath) {
        cout << "(" << point.first << ", " << point.second << ") ";
    }

    cout << "\nSize of Path is: " << dfsPath.size() - 1 << endl;

    for (auto point : dfsPath) {
        maze[point.first][point.second] = '.';
    }

    cout << "\nPath is: \n";
    printMaze(maze);

    return 0;
}
