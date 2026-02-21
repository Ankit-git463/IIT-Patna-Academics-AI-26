#include <bits/stdc++.h>
#define int  long long
#define test int t;cin>>t;while(t--)
#define yes cout<<"YES"<<endl
#define no cout<<"NO"<<endl
using namespace std;
 
const int M=1e9+7;
 

int help(vector<int>& a, vector<int>& b, int k) {
    int moves = 0;

    // Make all elements of 'a' >= k
    for (auto val : a) {
        if (val < k) {
            moves += (k - val);
        }
    }

    // Make all elements of 'b' <= k
    for (auto val : b) {
        if (val > k) {
            moves += (val - k);
        }
    }

    return moves;
}
 
void solve(){

    int n , m ; 
    cin>> n >> m ; 

    vector<int> a(n) , b(m) ;

    for(int i = 0 ; i< n; i++ )cin>>a[i]; 
    for(int i = 0 ; i< m; i++ )cin>>b[i]; 

    int ans = LLONG_MAX ; 

    int lo = 0 ; 
    int hi = 1e9 ; 

    while(lo <= hi ){

        int mid = (hi+lo) / 2 ;

        int cost_mid = help(a, b, mid);
        ans = min(ans, cost_mid);

        // Compare with neighbors to decide direction
        int cost_left = help(a, b, mid - 1);
        int cost_right = help(a, b, mid + 1);

        if (cost_left < cost_mid) {
            hi = mid - 1; // Move left
        } else if (cost_right < cost_mid) {
            lo = mid + 1; // Move right
        } else {
            break;
        }

        
    }

    cout<<ans <<endl ; 


    
    
}
 
signed main(){
 
    //do-not-use this in case of interactive
    ios_base::sync_with_stdio(0); //
    cin.tie(0);                   //
    //   //   //  //  //  //  //  //  
 
    solve();
 
}