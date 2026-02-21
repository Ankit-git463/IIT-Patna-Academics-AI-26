#include <bits/stdc++.h>
using namespace std;

// } Driver Code Ends
// User function Template for C++

class Solution{
public:
    int nCr(int n, int r){
        // code here
        int mod = 1000000007;
        if (r > n){
            return 0;
        }
        
        long long numerator = 1;
        long long denominator = 1;
        
        for (int i = 0; i < r; i++){
            numerator = (numerator * (n - i)) % mod;
            denominator = (denominator * (i + 1)) % mod;
        }
        
        // Calculate modular inverse of denominator
        long long inverse = 1;
        long long base = denominator;
        long long power = mod - 2;
        while (power > 0){
            if (power & 1){
                inverse = (inverse * base) % mod;
            }
            base = (base * base) % mod;
            power >>= 1;
        }
        
        // Calculate nCr modulo mod
        long long result = (numerator * inverse) % mod;
        return result;
    }
};


//{ Driver Code Starts.

int main(){
    int t;
    cin>>t;
    while(t--){
        int n, r;
        cin>>n>>r;
        
        Solution ob;
        cout<<ob.nCr(n, r)<<endl;
    }
    return 0;
}