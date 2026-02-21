#include <bits/stdc++.h>
#define int  long long
#define test int t;cin>>t;while(t--)
#define yes cout<<"YES"<<endl
#define no cout<<"NO"<<endl
using namespace std;
 
const int M=1e9+7;
 
void solve(){

    int  n; 
    cin>> n ; 

    
    int N = 1e5+1 ; 
    vector<int> v(N , 0 ) ;
    
    for (int i = 0 ; i< n ; i++) {
        int x ; 
        cin>>x ; 
        v[x]++ ; 
    }
    
    vector<int> dp(N, 0); 

    dp[1]= v[1]; 

    for(int i = 2 ; i<N ; i++){
        dp[i] = max(dp[i-1] , dp[i-2]+ v[i]*i);
    }

    cout<<dp[N-1]<<endl;

   
    
    
}
 
signed main(){
 
    //do-not-use this in case of interactive
    ios_base::sync_with_stdio(0); //
    cin.tie(0);                   //
    //   //   //  //  //  //  //  //  
 
    solve() ; 
 
}