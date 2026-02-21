#include <bits/stdc++.h>
#define int long long
#define test int t; cin >> t; while (t--)
#define yes cout << "YES" << endl
#define no cout << "NO" << endl
using namespace std;

const int M = 1e9 + 7;

int n;
vector<vector<int>> e(27, vector<int>(27, 0)); 
vector<string> s(101); 
vector<bool> visited(27, false);


int getID(char c) {
    if (c == ' ') return 0; // Space maps to ID 0
    return c - 'a' + 1; // Map 'a'-'z' to 1-26
}

void solve() {
    
    for (int i = 1; i <= 26; i++) {
        e[0][i] = 1;
    }

    
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> s[i];
        s[i] += " "; 
    }

   
    for (int i = 1; i < n; i++) {
        int pos = 0;
        // Find the first differing character position
        while (s[i][pos] == s[i + 1][pos]) pos++;
       
        e[getID(s[i][pos])][getID(s[i + 1][pos])] = 1;
    }

    // Use Floyd-Warshall algorithm to find transitive closure of the graph
    for (int k = 0; k <= 26; k++) {
        for (int i = 0; i <= 26; i++) {
            for (int j = 0; j <= 26; j++) {
                e[i][j] = e[i][j] || (e[i][k] && e[k][j]);
            }
        }
    }

    // Check for cycles in the graph
    bool haveCycle = false;
    for (int i = 0; i <= 26; i++) {
        if (e[i][i]) {
            haveCycle = true;
            break;
        }
    }

    if (haveCycle) {
       
        cout << "Impossible" << endl;
    } else {
   
        for (int i = 0; i <= 26; i++) {
            int now = -1;
            for (int j = 0; j <= 26; j++) {
               
                if (!visited[j]) {
                    bool valid = true;
                    for (int k = 0; k <= 26; k++) {
                        if (!visited[k] && e[k][j]) {
                            valid = false; 
                            break;
                        }
                    }
                    if (valid) {
                        now = j;
                        break;
                    }
                }
            }
            if (now == -1) break; 
            if (i > 0) cout << char('a' + now - 1); 
            visited[now] = true; 
        }
        cout << endl;
    }
}

signed main() {
  
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    solve(); 
}
