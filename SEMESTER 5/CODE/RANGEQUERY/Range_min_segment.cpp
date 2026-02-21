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

class SegmentTree{
    public : 

    vector <int> tree ; 
    int n ; 

    SegmentTree(vector <int> &arr){
        n = arr.size();
        tree.resize(2*n);

        build(arr);
    }

    void build(vector<int> & arr){
        for (int i = 0 ; i< n ;i++) {
            tree[n+i] = arr[i];
        }

        for(int i = n-1 ; i>0 ; i-- ){
            tree[i] = min(tree[i*2] , tree[2*i+1 ]);
        }
    }

    int rangemin(int l ,int r ){
        l+=n ; 
        r+=n+1; 

        int mini = INT_MAX;
        while(l<r){

            if (l%2 == 1 ){
                mini= min(mini , tree[l] );
                l++;
            }

            if (r%2 == 1 ){
                r-- ;
                mini = min(mini , tree[r]);
            }

            l=l/2 ; 
            r=r/2; 
        }

        return mini ;
    }
};
 
signed  main(){  

    int n ,q ; 

    cin>>n >>q ; 

    vector <int> v(n);

    for(int i = 0 ; i< n ; i++ )cin>>v[i];

    SegmentTree tr(v);

    for(int i = 0 ; i< q ; i++ ){
        int a , b; 
        cin>>a >> b ;

        cout<<tr.rangemin(a-1 , b-1)<<endl; 
    }


}