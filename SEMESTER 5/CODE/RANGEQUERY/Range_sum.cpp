#include <bits/stdc++.h>

using namespace std;

const int N=998244353;
typedef long long int ll; 
#define int long long int 
#define pb push_back
#define f first
#define s second
#define iS it->second
#define iF it->first
#define vi vector<int> 
#define vl vector<ll> 
#define vs vector<string> 

#define all(v)  v.begin(),v.end()
#define print(i,nx) for(ll i=0;i<n;i++)
#define read(v) for(auto it=v.begin();it!=v.end();it++){cout<<*it<<" ";}cout<<endl;
#define ios ios_base::sync_with_stdio(false);cin.tie(0);cout.tie(0);
#define yes cout << "YES" << endl ;
#define no cout << "NO" << endl ;
 
//FOR DEBUGGING PURPOSE
#define printarr(arr) for(int i=0;i<(sizeof(arr)/sizeof(arr[0]));i++){ cout << arr[i] << " ";}cout << endl;
#define printvec(v) for(int i=0;i<v.size();i++){ cout << v[i] << " ";}cout << endl;
#define printmap(m) for(auto i: m){ cout << i.first << " " << i.second << endl;}
#define printvecpair(v) for(int i=0;i<v.size();i++){cout << v[i].first << " " << v[i].second << endl;}
 
 
signed main(){  
    int n , q ; 

    cin>>n >>q ; 

    vector <int> v(n);
    for(int i= 0 ; i< n ;i++ )cin>>v[i]; 

    vector <int> prefix(n,0 );
    prefix[0] = v[0]; 

    for (int i = 1 ; i< n; i++ )prefix[i] +=prefix[i-1]+v[i];

    for(int  i= 0 ; i< q ; i++ ){
        int a, b ;
        cin>>a>>b ; 

        if (a== 1  ){
            cout<<prefix[b-1]<<endl;
        }

        else {
            cout<<prefix[b-1]-prefix[a-2]<<endl;
        }


    }


}