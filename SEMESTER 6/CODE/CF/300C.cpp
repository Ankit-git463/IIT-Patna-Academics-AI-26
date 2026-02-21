#include <bits/stdc++.h>
#define int  long long
#define test int t;cin>>t;while(t--)
#define yes cout<<"YES"<<endl
#define no cout<<"NO"<<endl
using namespace std;
 
const int M=1e9+7;
 
int nCr(int n, int r){
    
    int mod = 1e9+7; 
    int inverse =  1 ; 
    

    int num = 1 ; 
    int den = 1 ; 

    for(int i = 0 ; i< r ; i++ ){
        num = (num*(n-i) % mod ); 
        den = (den*(r-i) % mod ); 
    }


    int base = den ;
    int power = mod-2 ;

    while(power > 0){

        if(power% 2 == 1 ){
            inverse = (base*inverse)% mod ; 
        }

        base = (base*base)%mod ; 
        power >>= 1 ; 

    } 

    int res = (inverse*num) %mod ; 

    return res ; 


}
//___________________________________________________________________//


bool goodnum(int n  ,int a , int b ){
    int sum = n ;
    while(sum>0){
        int d= sum%10 ; 
        if(d==a || d==b ){
            // do nothing 
        }

        else {
            return 0;
        }

        sum=sum/10;
    }

    return 1 ; 

    
}
void solve(){

    int a , b , n ; 
    cin>>a>>b>>n ; 

    int ans = 0 ;

    for(int i = 0 ;i<=n ; i++ ){
        int sum = a*i + b*(n-i) ; 
        if (goodnum(sum , a, b) ){
            ans = (ans %M + nCr(n , min(i , n-i) )%M)%M ; 
        }
    }

    cout<<ans<<endl;
    
}
 
signed main(){
 
    //do-not-use this in case of interactive
    ios_base::sync_with_stdio(0); //
    cin.tie(0);                   //
    //   //   //  //  //  //  //  //  
 
    solve();
 
}