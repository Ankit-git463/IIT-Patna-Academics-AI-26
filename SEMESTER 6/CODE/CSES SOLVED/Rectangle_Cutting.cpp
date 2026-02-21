#include <bits/stdc++.h>
using namespace std ; 

vector < vector <int>> dp; 
int solve(int a , int b ){

    // Base Case :
    if(a== b ){
        return 0;
    }

    if( a == 1 )return b-1 ; 
    if( b == 1 )return a-1 ; 
   
    if(dp[a][b] != -1 ) return dp[a][b] ; 
    int mincuts = 1e9 ; 

    for (int i = 1 ; i<=a/2 ; i++ ){
        int cuts = 1+ solve(i, b ) + solve(a-i , b ); 
        mincuts = min(mincuts , cuts) ; 
    }

    for (int i = 1 ; i<=b/2 ; i++ ){
        int cuts = 1+ solve(a, i ) + solve(a , b-i ); 
        mincuts = min(mincuts , cuts) ; 
    }

    return dp[a][b] = mincuts; 

}

int main(){

    int a ,b ; 
    cin>>a>>b ; 

    dp.assign(a+1 , vector <int> (b+1 , -1)); 

    cout<<solve(a,b)<<endl;

}