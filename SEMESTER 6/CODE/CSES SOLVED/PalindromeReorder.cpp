#include<bits/stdc++.h>
using namespace std ; 

typedef vector<vector <int > > vvi ;
typedef vector<int> vi ; 

#define  print(vector) for (auto it: vector) cout<<it<<" " ; 

int main(){
    string s ;
    cin>>s;

    vector <int> f(26,0 ); 
    for ( char c : s ){
        f[c-'A'] ++ ;
    }

    int n = s.size() ; 

    char oddchar  ;
    if(n%2 == 0 ){
        // all should be even 
        for (auto val : f) {
            if(val%2 == 1 ){
                cout<<"NO SOLUTION"<<endl; 
                return 0 ; 
            }
        }
    }

    else{ 

        // only one should be odd 
        int odd = 0 ; 
        
        for (int i = 0 ; i< 26 ; i++ ) {
            int val  = f[i] ; 
            if(val%2 == 1 ){
                odd++ ; 
                oddchar= i+'A'; 
            }
            
        }

        if(odd != 1 ){
            cout<<"NO SOLUTION"<<endl; 
            return 0 ; 
        }

        
        s[n/2] = oddchar; 
        
        f[oddchar-'A']--; 
        

    }


    int x = 0 ; 

    for(int i = 0 ; i< 26 ; i++ ){
        while(f[i]!= 0 ){
            s[x]=(i+'A');
            s[n-x-1] = s[x];
            x++;
            f[i]-=2;
            
        }
    }

    cout<<s<<endl;

    


}