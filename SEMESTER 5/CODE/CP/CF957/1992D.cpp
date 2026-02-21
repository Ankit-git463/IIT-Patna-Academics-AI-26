#include <bits/stdc++.h>
#define int long long
#define test int t; cin >> t; while (t--)
#define yes cout << "YES\n"
#define no cout << "NO\n"
using namespace std;

const int M = 1e9 + 7;

vector<int> dp;  

bool sol(const string &str, int i, int n, int m, int swim) {
    if (swim <0) return false;
    if (i + m >= n-1) return true; 

    if (dp[i] != -1) return dp[i];

    bool jump = false;
    bool sw = false;

    for (int j = i + 1; j <= min(n - 2, i + m); j++) {
        if (str[i]=='L' and str[j] == 'L' ) {
            jump |= sol(str, j, n, m, swim);
        }
    }

    if (str[i + 1] == 'W' && swim > 0) {
        sw = sol(str, i + 1, n, m, swim - 1);
    }

    return dp[i] = (jump || sw);  
}

void solve() {
    int n, m, k;
    cin >> n >> m >> k;

    string str;
    cin >> str;

    str = 'L' + str+ 'L';  
    n+=2; 

    dp.assign(n, -1);  

    if (sol(str, 0, n, m, k)) {
        yes;
    } else {
        no;
    }
}

signed main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    test {
        solve();
    }

    return 0;
}
