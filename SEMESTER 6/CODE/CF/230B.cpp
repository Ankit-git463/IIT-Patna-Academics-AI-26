#include <bits/stdc++.h>
#define int  long long
#define test int t;cin>>t;while(t--)
#define yes cout<<"YES"<<endl
#define no cout<<"NO"<<endl
using namespace std;
 
const int M=1e9+7;

//___________________________________________________________________//
 
//FOR DEBUGGING
#define printarr(arr) for(int i=0;i<(sizeof(arr)/sizeof(arr[0]));i++){ cout << arr[i] << " ";}cout << endl;
#define printvec(v) for(int i=0;i<v.size();i++){ cout << v[i] << " ";}cout << endl;
#define printset(s) for(auto i : s){cout << i << " ";}cout<<endl;
 
//____________________________________________________________________//
 
int help(int n ){

    int lo = 0 ; 
    int hi = 1e9 ; 

    while(lo<= hi ){

        int mid = (hi-lo)/2 + lo  ;

        if( mid * mid <= n ){
            lo = mid +1 ;
        } 

        else {
            hi = mid-1 ; 
        }
    }

    return hi ;
}

void solve(){

    int n ; 
    cin>> n ; 

    int limit= 1e6;
    vector<int> isPrime(limit+1 , true);

    isPrime[0] = isPrime[1] = false; // 0 and 1 are not prime numbers

    for (int i = 2; i * i <= limit; ++i) {
        if (isPrime[i]) {
            for (int j = i * i; j <= limit; j += i) {
                isPrime[j] = false;
            }
        }
    }

   

    for(int i = 0 ; i< n ; i++ ){
        int x ; 
        cin>>x ; 

        if(x==1){
            no;
            continue;
        }

       
        int y= help(x); 
        int sq = y*y; 

        if(sq== x ) {
            if(isPrime[y])yes;
            else no ; 
        }
        else{
            no;
        }
    }
    
    
}
 
signed main(){
 
    //do-not-use this in case of interactive
    ios_base::sync_with_stdio(0); //
    cin.tie(0);                   //
    //   //   //  //  //  //  //  //  
 
    solve();

    
 
}