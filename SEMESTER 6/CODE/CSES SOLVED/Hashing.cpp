# include<bits/stdc++.h>
using namespace std ; 

typedef vector<vector <int > > vvi ;
typedef vector<int> vi ; 

#define  print(vector) for (auto it: vector) cout<<it<<" " ; 
#define int long long int
int MOD = 1e9+7 ; 

int makehash(string s ){
    int x = 31 ; 
    int ans = 0 ; 
    for (char  c: s ){
        ans = (ans*31 + (c-'a' +1 )) % MOD ;  
    }
    return ans ; 
}
signed main(){

    cout<<makehash("ans")<<endl; 
    
}