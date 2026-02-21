# include<bits/stdc++.h>
using namespace std ; 

typedef vector<vector <int > > vvi ;
typedef vector<int> vi ; 

#define  print(vector) for (auto it: vector) cout<<it<<" " ; 

set<int>ans ;
int n ; 

// dp[i][sum] represents whether it is possible to achieve the value sum using the first i coins (from index 0 to i-1).

// tackling 1 1 1 1 1 1 1 1.....
vector<vector <int>> dp ;
bool solve(int i, vector <int> &arr ,int sum  ){
    
    if(i== n ){
        if(sum> 0 )ans.insert(sum) ; 
        return true  ; 
    }

    if(dp[i][sum]!= -1 )return dp[i][sum] ; 
    
    bool take = false ;
    take = solve(i+1, arr, sum-arr[i]);
    bool nottake = solve(i + 1 , arr ,sum ); 

    return dp[i][sum] = take || nottake ; 

}

int main(){
     
    cin>>n ; 

    vector <int> v(n);

    int sum = 0 ; 

    for (int i=0 ; i<n ; i++){
        cin>>v[i];
        sum+=v[i];
    }


    dp.assign(n+1 , vector <int> (sum+1 , -1 )); 

    solve(0 , v , sum );

    cout<<ans.size()<<endl;

    for(auto val : ans) cout<<val<<" "; 
    cout<<endl ;










}