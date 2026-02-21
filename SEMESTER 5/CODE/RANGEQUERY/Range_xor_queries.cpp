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

class Seg{
    public:

    int n ; 
    vi tree; 

    Seg(vi &arr){
        n= arr.size();
        tree.resize(2*n);

        build(arr);
    }

    void build(vi &arr){
        for (int i = 0 ; i< n ;i++ ){
            tree[i+n] = arr[i];
        }

        for (int i = n-1 ; i>0 ; i--){
            tree[i] = tree[2*i]^tree[2*i+1];
        }
    }

    int rangexor( int l ,int r ){
        l+=n ; 
        r+=n+1 ; 

        int xr = 0 ; 
        while(l<r){
            if(l%2 == 1 ){
                xr^=tree[l];
                l++;
            }

            if (r%2 == 1 ){
                r--;
                xr^=tree[r];
            }

            l/=2;
            r/=2;
        }

        return xr;

    }

};

 
signed main(){  
    int n , q ;
    cin>>n>>q ; 

    vi v(n);
    for (int i = 0 ; i< n; i++)cin>>v[i];

    Seg tr(v);

    for (int i = 0 ; i< q ; i++ ){
        int a ,b ; 
        cin>>a>>b ; 

        cout<<tr.rangexor(a-1 , b-1 )<<endl;
    }




}