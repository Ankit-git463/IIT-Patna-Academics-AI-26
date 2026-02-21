#include <bits/stdc++.h>
#define int long long
#define test int t; cin >> t; while (t--)
#define yes cout << "YES" << endl
#define no cout << "NO" << endl
using namespace std;

const int M = 1e9 + 7;

int n;
vector<int> rangemin;
vector<int> rangegcd;

// Build the rangemin array
void buildmin(int size, vector<int> &v) {
    n = size;
    rangemin.resize(2 * n);

    for (int i = 0; i < n; i++) {
        rangemin[i + n] = v[i];
    }

    for (int i = n - 1; i > 0; i--) {
        rangemin[i] = min(rangemin[2 * i], rangemin[2 * i + 1]);
    }
}

// Build the rangegcd array
void buildgcd(int size, vector<int> &v) {
    n = size;
    rangegcd.resize(2 * n);

    for (int i = 0; i < n; i++) {
        rangegcd[i + n] = v[i];
    }

    for (int i = n - 1; i > 0; i--) {
        rangegcd[i] = __gcd(rangegcd[2 * i], rangegcd[2 * i + 1]);
    }
}

// Query minimum in range [l, r)
int querymin(int l, int r) {
    int left = l + n;
    int right = r + n;
    int mi = INT_MAX;

    while (left < right) {
        if (left % 2 == 1) {
            mi = min(mi, rangemin[left]);
            left++;
        }
        if (right % 2 == 1) {
            right--;
            mi = min(mi, rangemin[right]);
        }
        left /= 2;
        right /= 2;
    }
    return mi;
}

// Query GCD in range [l, r)
int querygcd(int l, int r) {
    int left = l + n;
    int right = r + n;
    int res = 0;

    while (left < right) {
        if (left % 2 == 1) {
            res = __gcd(res, rangegcd[left]);
            left++;
        }
        if (right % 2 == 1) {
            right--;
            res = __gcd(res, rangegcd[right]);
        }
        left /= 2;
        right /= 2;
    }
    return res;
}

// Check if any subarray of length `len` satisfies the condition
bool help(vector<int> &v, int len) {
    int n = v.size();
    int left = 0;
    int right = left + len - 1;

    while (right < n) {
        if (querygcd(left, right + 1) == querymin(left, right + 1)) {
            return true;
        }
        left++;
        right++;
    }
    return false;
}


void solve() {
    int m;
    cin >> m;

    vector<int> v(m);
    for (int i = 0; i < m; i++) {
        cin >> v[i];
    }

    buildgcd(m, v);
    buildmin(m, v);

    int lo = 1;
    int hi = m;
    int anslen = 0;

    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (help(v, mid)) {
            anslen = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }

    vector<int> ans;
    int left = 0;
    int right = left + anslen - 1;

    while (right < m) {
        if (querygcd(left, right + 1) == querymin(left, right + 1)) {
            ans.push_back(left + 1);
        }
        left++;
        right++;
    }

    cout << ans.size() << " " << anslen-1 << endl;
    for (auto val : ans) {
        cout << val << " ";
    }
    cout << endl;
}

signed main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    solve();
}
