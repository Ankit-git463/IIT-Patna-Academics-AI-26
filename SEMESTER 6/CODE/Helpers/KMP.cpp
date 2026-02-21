# include<bits/stdc++.h>
using namespace std ; 

// Function to compute the LPS array
vector<int> computeLPS( string t) {
    int m = t.size() ; 
    vector<int> lps(m, 0);

    int i=1 ; 
    int len = 0 ; 

    while(i< m ){

        if( t[i] == t[len] ){
            len ++ ; 
            lps[i] = len ; 
            i++ ; 

        }
        else{
            if(len != 0 ){
                len = lps[len-1];
            }

            else{
                lps[i] = 0 ;
                i++;
            }
        }
    } 

    return lps;
}
 


// KMP search function
bool search( string &text, const string &pattern) {
    vector<int> lps = computeLPS(pattern);
    int i = 0, j = 0;

    while (i < text.size()) {
        if (text[i] == pattern[j]) {
            i++;
            j++;
        }

        if (j == pattern.size()) {
            return true; // Pattern found
        } 
        
        else if (i < text.size() && text[i] != pattern[j]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
    return false; // Pattern not found
}
 

int main(){
    
}