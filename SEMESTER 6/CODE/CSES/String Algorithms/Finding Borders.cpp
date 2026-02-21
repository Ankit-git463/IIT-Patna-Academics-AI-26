# include<bits/stdc++.h>
using namespace std ; 

typedef vector<vector <int > > vvi ;
typedef vector<int> vi ; 

#define  print(vector) for (auto it: vector) cout<<it<<" " ; 
#define int long long int 

int n ; 
vector <int > ans ; 

// O(n+m )
void solve(string s , int m , int hp ){

    int p = 1 ; 

    int prefix = 0 ; 
    int suffix = 0 ; 

    for (int i = 0 ; i< n-1 ; i++ ){
        prefix =( prefix*hp + (s[i]-'a'+1 )) % m ; 
        suffix = (suffix + (s[n-i-1]-'a'+1 )*p ) % m ; 

        

        if(prefix == suffix ){
            ans[i]++; 
        }

        p = (p*hp)% m ; 
    }


}

signed main(){

    string s ; 
    cin>>s ; 
    n = s.size() ; 

    ans.assign(n , 0 ); 

    solve(s , 1e9+7 , 31 ); 
    solve(s , 1e9+9 , 9983 ); 
    solve(s , 1e9+7 , 83 ); 

    for (int i =0 ; i< n ; i++  ){
        if (ans[i] == 3){
            cout<<i+1<<" "; 
        }
    }

    cout<<endl; 
    
}