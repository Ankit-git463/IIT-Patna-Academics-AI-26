#include <bits/stdc++.h>
#define int  long long

using namespace std;
 
const int M=1e9+7;

void solve(){

    int n , m ; 
    cin>> n>>m ; 


    int x = 0 ; 
    int y = m ; 

    vector <pair<int, int> > ans ; 

    while( x <= n and y>=0 ){
        ans.push_back({x,y}); 
        x++;
        y--;   
    }

    cout<< ans.size() <<endl; 
    for(auto p : ans){
        cout<<p.first << " "<< p.second<<endl;
    }
    
    
}
 
signed main(){
 
    //do-not-use this in case of interactive
    ios_base::sync_with_stdio(0); //
    cin.tie(0);                   //
    //   //   //  //  //  //  //  //  
 
    solve() ; 
 
}