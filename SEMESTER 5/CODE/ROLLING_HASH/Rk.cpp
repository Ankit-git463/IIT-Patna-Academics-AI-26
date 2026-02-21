#include <bits/stdc++.h>
using namespace std;

#define int long long int 
// d is the number of characters in the input alphabet
#define d 256

/* 
 * Function to perform the Rabin-Karp algorithm
 * 
 * pat -> pattern to search for
 * txt -> text to search within
 * q -> a prime number for hashing
 */
void search(const char pattern[], const char text[], int prime) {
    int patternLength = strlen(pattern);
    int textLength = strlen(text);
    int i, j;
    int patternHash = 0; // hash value for pattern
    int textHash = 0;    // hash value for text
    int h = 1;           // value of d^(M-1) % q

    // Calculate the value of h (d^(M-1) % q)
    for (i = 0; i < patternLength - 1; i++) {
        h = (h * d) % prime;
    }

    cout<<h <<endl; 

    // Calculate the hash value of pattern and the first window of text
    for(i = 0; i < patternLength; i++) {
        patternHash = (d * patternHash + pattern[i]) % prime;
        textHash = (d * textHash + text[i]) % prime;
    }

    // Slide the pattern over the text one by one
    for (i = 0; i <= textLength - patternLength; i++) {
        // Check the hash values of the current window of text and pattern
        if (patternHash == textHash) {
            // Check for characters one by one
            for (j = 0; j < patternLength; j++) {
                if (text[i + j] != pattern[j]) {
                    break; // Mismatch found
                }
            }

            // If p == t and pattern matches the text
            if (j == patternLength) {
                cout << "Pattern found at index " << i << endl;
            }
        }

        // Calculate hash value for the next window of text
        if (i < textLength - patternLength) {
            textHash = (d * (textHash - text[i] * h) + text[i + patternLength]) % prime;

            // Adjust if we get a negative value of textHash
            if (textHash < 0) {
                textHash += prime;
            }
        }
    }
}

/* Driver code */
signed main() {
    char text[] = "GEEKS FOR GEEKS";
    char pattern[] = "GEE";

    // Use a large prime number to avoid collisions
    int prime = 998244353;

    // Function Call
    search(pattern, text, prime);
    return 0;
}
