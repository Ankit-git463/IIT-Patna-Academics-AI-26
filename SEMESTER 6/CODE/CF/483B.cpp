#include <bits/stdc++.h>
#define int  long long
#define test int t;cin>>t;while(t--)
#define yes cout<<"YES"<<endl
#define no cout<<"NO"<<endl
using namespace std;
 
const int M=1e9+7;

 
int help(int v , int cnt1 , int cnt2 , int x , int y ){

    int a = v/x ; 
    int b = v/y ; 

    int common = v / (x*y);

    int first = b-common; 
    int second = a - common ; 

    int left = v- (a+b - common) ;

    int reqforfirst = 0;
    if(cnt1 > first ){
        reqforfirst = cnt1-first ; 

        if(left < reqforfirst)return false;
    }

    left = left- reqforfirst ; 

    if(left+ second >= cnt2 )return true ; 
    return false ; 

}
 
void solve(){

    int cnt1 , cnt2 , x ,y ; 

    cin>> cnt1 >> cnt2 >> x >> y ; 

    int lo = 1 ; 
    int hi = 1e16+1 ; 

    while( lo <= hi ){
        int mid = (lo+hi)/2 ; 


        if( help(mid , cnt1 , cnt2 , x , y ) == false ){
            lo = mid + 1 ; 
        }

        else{
            hi = mid - 1 ; 
        }
    }

    

    cout<<lo <<endl ; 




    
    
}
 
signed main(){
 
    //do-not-use this in case of interactive
    ios_base::sync_with_stdio(0); //
    cin.tie(0);                   //
    //   //   //  //  //  //  //  //  
 
    solve() ; 
 
}