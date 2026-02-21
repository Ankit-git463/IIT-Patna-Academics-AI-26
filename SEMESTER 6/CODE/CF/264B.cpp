#include <bits/stdc++.h>
#define int  long long
#define test int t;cin>>t;while(t--)
#define yes cout<<"YES"<<endl
#define no cout<<"NO"<<endl
using namespace std;

 
void solve() {
    int n;
    cin >> n;
    vector<int> v(n);
    for(int i = 0; i < n; i++) cin >> v[i];
    
    vector<int> lis(n, 1);
    
    // Precompute GCD for faster access
    vector<vector<int>> gc(n, vector<int>(n));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            gc[i][j] = __gcd(v[i], v[j]);
        }
    }
    
    // Main DP loop
    for(int i = 1; i < n; i++) {
        for(int j = 0; j < i; j++) {
            // Check if current element is greater and has GCD > 1
            if(v[i] > v[j] && gc[i][j] > 1) {
                lis[i] = max(lis[i], lis[j] + 1);
            }
        }
    }
    
    int ans = *max_element(lis.begin(), lis.end());
    cout << ans << endl;
}


 
signed main(){
 
    //do-not-use this in case of interactive
    ios_base::sync_with_stdio(0); //
    cin.tie(0);                   //
    //   //   //  //  //  //  //  //  
 
    solve() ; 
 
}