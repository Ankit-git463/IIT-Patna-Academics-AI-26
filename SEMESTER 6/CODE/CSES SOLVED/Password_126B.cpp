#include <bits/stdc++.h>
#define int  long long
#define test int t;cin>>t;while(t--)
#define yes cout<<"YES"<<endl
#define no cout<<"NO"<<endl
using namespace std;
 
const int M=1e9+7;
 
int power(int a,int b){
    if(b==0) return 1;
    int tmp=power(a,b/2);
    int result=((tmp%M)*(tmp%M))%M;
    if(b%2==1) result =((result%M)*(a%M))%M;
    return result;
}
 
int mul(int a,int b){
    return ((a%M)*(b%M)%M);
}

//___________________________________________________________________//
 
//FOR DEBUGGING
#define printarr(arr) for(int i=0;i<(sizeof(arr)/sizeof(arr[0]));i++){ cout << arr[i] << " ";}cout << endl;
#define printvec(v) for(int i=0;i<v.size();i++){ cout << v[i] << " ";}cout << endl;
#define printset(s) for(auto i : s){cout << i << " ";}cout<<endl;
 
//____________________________________________________________________//
 
 
void solve(){
    string s ; 
    cin>>s ;

    
    
}
 
signed main(){
 
    //do-not-use this in case of interactive
    ios_base::sync_with_stdio(0); //
    cin.tie(0);                   //
    //   //   //  //  //  //  //  //  
 
    
    solve();
    
 
}