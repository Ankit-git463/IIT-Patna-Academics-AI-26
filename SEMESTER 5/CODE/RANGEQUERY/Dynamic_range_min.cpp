#include <bits/stdc++.h>

using namespace std;

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
    public : 

    int n ; 
    vi tree ; 

    Seg(vi & arr){
        n= arr.size();
        tree.resize(2*n);

        build(arr);
    }

    void build(vi &arr){
        for (int i = 0 ; i< n ;i ++ ){
            tree[n+i] = arr[i];
        }

        for (int i = n-1 ; i>0 ; i-- ){
            tree[i] = min(tree[i*2] , tree[2*i +1 ]);
        }
    }

    int rangemin( int l , int r ){
        l+=n ; 
        r+=n+1 ; 

        int mini = INT_MAX ; 
        while(l<r){

            if (l%2 == 1 ){
                mini = min (mini , tree[l]);
                l++;
            }

            if (r%2 == 1 ){
                r-- ; 
                mini = min(mini , tree[r]);
            }

            l/=2;
            r/=2 ; 

        }

        return mini ; 
    }

    void update(int pos ,int value ){
        pos+= n ; 

        tree[pos] = value ;

        for (int i = pos ; i>1 ; i/=2){
            tree[i/2 ] = min( tree[i] , tree[i^1]);
        }
    }
};
 
signed main(){  
    int n , q ; 
    cin>>n>>q ; 

    vi v(n);
    for (int i = 0 ; i<n ;i++)cin>>v[i];

    Seg tr(v);

    for (int i = 0 ; i<q ; i++ ){
        int a ,b,c ;

        cin>>a>>b>>c ;

        if (a==1 ){
            tr.update(b-1 , c);
        }

        else{
            cout<< tr.rangemin(b-1 , c-1 )<<endl;
        }


    }

}