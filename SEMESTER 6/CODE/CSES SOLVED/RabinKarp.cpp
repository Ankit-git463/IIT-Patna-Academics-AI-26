# include<bits/stdc++.h>
using namespace std ; 

typedef vector<vector <int > > vvi ;
typedef vector<int> vi ; 

#define  print(vector) for (auto it: vector) cout<<it<<" " ; 
#define int long long int 
// ------------------------------------------------------------------------------------------------
const int M = 1e9 +7 ; 
const int hp = 31 ; 


int genhash(string s , int hp ){
    int ans = 0 ; 

    // calculating the hash value 
    for (auto c: s ){
        ans = (ans  * hp + (c-'a' +1 )) % M ; 
    }
    return ans ; 
}

vector <int> v ; 

void  rabinkarp (string s , string t ,int  hp ){
    int n = s.size( ); 
    int m = t.size( ); 
    int p = 1 ; 

    // calculating the highest power in pattern
    for  (int i = 0 ; i< m-1 ; i++ ){
        p = (p*hp) % M ; 
    }
    // pattern hash 
    int ht = genhash(t , hp ); 
    // texthash 
    int hs = genhash(s.substr(0, m   ) , hp ); 
    // zero index match check 
    if (hs == ht )v[0]+= 1;
    // rolling the hash 

    for (int l= 1 , r = m ; r< n ; r++ , l++ ){
        // abcde -->>  bcdef
        int del = ((s[l-1] - 'a' +1) * p)% M ; 
        int add = s[r]-'a' +1  ; 

        // delete the first char 
        hs = ( (hs-del+ M ) * hp )% M ; 

        // add the new one  
        hs = (hs+add) %M ; 

        // checking the strings 
        if (hs == ht ){
            v[l]+=1 ; 
        }
    }
}

signed main(){

    string s , p  ; 
    cin>>s >> p ;

    v.assign(s.size(), 0 );

    // three times hashing use
    rabinkarp(s,p , 31 ) ; 
    rabinkarp(s,p , 37 ) ; 
    rabinkarp(s,p , 9973 ) ; 

    int count = 0 ; 
    for (int i  =0 ; i< s.size() ;i++ ){
        if(v[i] == 3 )count++ ; 
    }

    cout<<count<<endl;




    
}