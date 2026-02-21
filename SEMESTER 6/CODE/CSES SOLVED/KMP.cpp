#include <iostream>
#include <vector>
using namespace std;


vector <int> lps;
//build the LPS (Longest Prefix Suffix) array
void build(string pattern, vector<int>lps) {
    int n= pattern.size();
    int len = 0; // Length of the previous longest prefix suffix
    lps[0] = 0;  // LPS of the first character is always 0
    int i = 1;

    while (i < n) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1]; // Fall back in the pattern
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
}

// KMP search function
void KMPSearch(const string& text, const string& pattern) {
    int n = text.size();
    int m = pattern.size();

    // Build the LPS array
    vector<int> lps(m);
    build(pattern, lps);

    int i = 0; // Index for text
    int j = 0; // Index for pattern

    while (i < n) {
        if (pattern[j] == text[i]) {
            i++;
            j++;
        }

        if (j == m) {
            cout << "Pattern found at index " << i - j << endl;
            j = lps[j - 1];
        } else if (i < n && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
}

// Main function to demonstrate the KMP algorithm
int main() {
    string text = "ababcabcabababd";
    string pattern = "ababd";

    cout << "Text: " << text << endl;
    cout << "Pattern: " << pattern << endl;

    KMPSearch(text, pattern);

    return 0;
}
