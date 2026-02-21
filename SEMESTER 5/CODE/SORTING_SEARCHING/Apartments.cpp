#include <bits/stdc++.h>

using namespace std;

const int N=998244353;
typedef long long int ll; 
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
 
 
int main(){  

    int  n , m , k ; 
    cin>>n>>m >> k ;

    vector <int> app(n);
    vector <int> apar(m);

    for (int i = 0 ; i< n ;i++ )cin>>app[i]; 
    for (int i = 0 ; i< m ;i++ )cin>>apar[i];
   
    sort(app.begin() , app.end());
    sort(apar.begin() , apar.end());

    int i = 0 ; 
    int j = 0  ;

    int count = 0 ; 

    while(i< n and j<m ){

        if(app[i]>= apar[j]-k and app[i]<=apar[j]+k  ){
            i++;
            j++;
            count ++ ; 
        }

        if (app[i] < apar[j]-k){
            i++;
        }

        else if (app[i] > apar[j]+k){
            j++;
        }

    } 

    cout<< count  << endl ; 



}