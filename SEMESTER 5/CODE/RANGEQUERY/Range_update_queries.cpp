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


class Seg{
    public : 

    int n ; 
    vi tree; 
    vi prefix;
    

    Seg(vi &arr){
        int n = arr.size();
        tree.resize(n+1);
        prefix.resize(n+1);

        for  (int i = 0 ; i< n ; i++ ){
            tree[i]= arr[i];
        }
        prefix[0]= tree[0];
        for (int i = 1 ; i< n ;i++ ){
            prefix[i] += prefix[i-1];
        }
    }

    void update( int l ,int r , int u){
        l--;
        r--;
        vi lazysum(n , 0 ) ;
        lazysum[l]+=u;
        lazysum[r+1]-=u;

        prefix[0] = lazysum[0]+tree[0];
        for (int i = 1 ; i< n ;i++ ){
            prefix[i] += prefix[i-1]+lazysum[i];
        }

    }

    int query(int k ){
        k--;

        if(k==0 )return prefix[0];
        
        return prefix[k]-prefix[k-1];
        
    }

    

    
};
 
int main(){  
    int n , q ; 
    cin>>n >>q ; 

    vi v(n);
    for (int i = 0 ; i< n ;i++ )cin>>v[i] ; 

    Seg tr(v);

    while(q--){
        int a ; 
        cin>>a ; 

        if (a== 2 ){
            int k ; 
            cin>>k ; 
            cout<<tr.query(k)<<endl;
        }

        else{
            int x ,y , u ;
            cin>>x>>y>>u;
            tr.update(x,y,u);
        }
    }



}