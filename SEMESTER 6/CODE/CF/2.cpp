#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

bool check(vector<int>& a) {
    int n = a.size(), p = 0;

    for (int i = 0; i < n; i++) {
        bool f = false;
        vector<int> d;
        for (int j = 1; j * j <= a[i]; j++) {
            if (a[i] % j == 0) {
                d.push_back(j);
                if (j != a[i] / j) {
                    d.push_back(a[i] / j);
                }
            }
        }
        sort(d.begin(), d.end());
        for (int x : d) {
            if (x > p) {
                p = x;
                f = true;
                break;
            }
        }
        if (!f) return false;
    }
    return true;
}

int main() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    cout << (check(a) ? "YES" : "NO") << endl;
    return 0;
}
