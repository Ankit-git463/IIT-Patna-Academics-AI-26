# include<bits/stdc++.h>
using namespace std ; 

typedef vector<vector <int > > vvi ;
typedef vector<int> vi ; 

#define  print(vector) for (auto it: vector) cout<<it<<" " ; 
class Node{
    public : 
    Node *links[26] ; 
    bool flag = false ; 

    // function to check whether the key is present or not 
    bool containskey(char ch ){
        return links[ch-'a'] != NULL ; 
    }
    // add the node 
    void put(char ch , Node* node){
        links[ch-'a'] = node ; 
    }

    // function to get the node
    Node* get(char ch ){
        return links[ch-'a'];
    }

    // setend() 
    void setEnd(){
        flag = true;
    }

    // isEnd() > ----- <   
    bool isEnd(){
        return flag ; 
    }

}; 

class Trie {
    private : 
    Node * root ; 

    public: 

    Trie() { 
        root = new Node() ;
    }

    void insert(string word ){
        Node * node = root ; 
        // traverse the word 
        for(int i =0 ; i< word.length() ; i++ ){
            // if the character of the word is not present in the node
            // make the new node with that character 
            if(!node->containskey(word[i])){
                node->put(word[i] , new Node()); 
            }
            // go to the  next character in Trie  
            node  = node->get(word[i]);
        } 
        // set the flag to true ;
        node->setEnd();
    }

    bool search(string word){
        Node * node = root ; 
        // traverse the word 
        for (int i = 0 ; i< word.size() ; i++ ){
            // check if word[i] is present in the current node ;
            if ( !node->containskey(word[i]) )return false; 
            node = node->get(word[i]);
        }
        return node->isEnd();
    }

    bool startsWith(string word){
        Node * node = root ; 
        // traverse the word 
        for (int i =  0 ; i< word.size() ; i++ ){
            if(!node->containskey(word[i])){
                return false ;
            }
            node = node -> get(word[i]);
        }
        return true ;
    }

    bool printall(string word){
        Node * node = root ; 
        string str="";
        // traverse the word 
        for(int i = 0 ; i< word.size() ; i++ ){
            if(!node-> containskey(word[i]) ){
                return false ; 
            }
            str+=word[i] ; 
            // 
            node= node->get(word[i]); 
        }
    }
};

int main(){
    Trie *root = new Trie();

    root->insert("apple");
    root->insert("aeroplane");
    root->insert("arman"); 

    root->insert("bash"); 
    root->insert("barometer");
    root->insert("bashed"); 

    cout<<root->search("arman")<<endl;
    cout<<root->search("bash")<<endl;
    cout<<root->search("bas")<<endl;

    cout<<"StartsWith -> bas"<<endl;
    cout<<root->startsWith("bas")<<endl;

}