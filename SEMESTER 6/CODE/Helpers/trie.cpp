# include<bits/stdc++.h>
using namespace std ; 

typedef vector<vector <int > > vvi ;
typedef vector<int> vi ; 

#define  print(vector) for (auto it: vector) cout<<it<<" " ; 

class Node{
    public  : 
    
    Node * links[26]; 
    int flag ; 

    

    bool containskey(char ch ){
        return (links[ch-'a'] != NULL );
    }

    Node* get(char ch ){
        return links[ch-'a']; 
    }

    void put(char ch , Node* node ){
        links[ch-'a'] = node ; 
    }

    void setend(){
        flag = true ; 
    }

    bool isEnd() { 
        return flag ; 
    }

}; 

class Trie{

    Node * root ; 
    public:

    Trie(){
        root = new Node() ;
    }

    void insert(string word){

        int n = word.size() ; 
        Node * temp = root;

        for(int i = 0 ; i< n ; i++ ){
            if ( temp->containskey(word[i]) == false  ){
                temp->put(word[i] , new Node()); 
            }

            temp= temp->get(word[i]); 
        }

        temp->flag = true ; 
    }

    bool search(string word){

        int n = word.size() ; 
        Node * temp = root;

        for(int i = 0 ; i< n ; i++ ){
            if ( temp->containskey(word[i]) == false  ){
                return false ; 
            }

            temp= temp->get(word[i]); 
        }

        return temp->flag; 

    }

    bool startswith(string word ){
        int n = word.size() ; 
        Node * temp = root;

        for(int i = 0 ; i< n ; i++ ){
            if ( temp->containskey(word[i]) == false  ){
                return false ; 
            }

            temp= temp->get(word[i]); 
        }

        return true ;
    }
        
};

int main(){

    Trie t;

    t.insert("absdc");
    t.insert("abefc");
    t.insert("aadedc");
    t.insert("absedc");

    cout<<t.search("absed");
    
}