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
    vi tree ;

    Seg(vi & arr){
        n =arr.size();
        tree.resize(2*n);

        build(arr);
    } 

    void build(vi &arr){

        for (int i = 0 ; i< n ;i++ ){
            tree[n+i] = arr[i];
        }

        for (int i = n-1 ; i> 0 ; i-- ){
            tree[i] = tree[i*2] + tree[2*i+1];
        }

    }

    int rangesum(int l , int r ){
        l = l+n ; 
        r= r+ n+1 ; 
        int sum = 0 ;
        while(l<r){

            if (l%2 == 1 ){
                sum+=tree[l];
                l++;
            }

            if(r%2 == 1 ){
                r--;
                sum+=tree[r];
            }

            l/=2;
            r/=2;
            
        }

        

        return sum ;
    }

    void update(int pos , int val ){
        pos+= n ;
        tree[pos] =val ;

        for(int i = pos ; i>1 ; i/=2){
            tree[i/2] = tree[i] + tree[i^1];
        }
    
    }

    
};
 
signed main(){  

    int n , q ; 
    cin>> n >> q ;

    vi v(n);
    for (int i = 0 ; i< n ;i++ )cin>>v[i];

    Seg tr(v);

    for (int i = 0 ; i<q ; i++ ){
        int a ,b, c ;

        cin>>a>>b>>c ;

        if (a == 1){
            tr.update(b-1,c);
        }
        else {
            cout<<tr.rangesum(b-1 , c-1 )<<endl;
        }
    }



}