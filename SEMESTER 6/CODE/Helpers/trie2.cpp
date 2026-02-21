# include<bits/stdc++.h>
using namespace std ; 

typedef vector<vector <int > > vvi ;
typedef vector<int> vi ; 

#define  print(vector) for (auto it: vector) cout<<it<<" " ; 

class Node{

    public : 
    Node* links[26]; 
    int cw = 0 ; 
    int ew = 0 ; 

    bool present(char ch ){
        return (links[ch-'a'] != NULL ); 
    }

    Node* next(char ch ){
        return links[ch-'a'];
    }

    void add(char ch ){
        links[ch-'a'] = new Node() ; 
    }

    
    
};

class Trie{

    Node * root ; 

    public : 

    Trie(){
        root = new Node() ; 
    }

    void insert( string word){

        Node * temp = root ; 
        int n = word.size() ; 

        for(int i = 0 ; i< n ; i++ ){

            if(temp->present(word[i]) == false ){
                temp->add(word[i]) ;
            }

            temp= temp->links[word[i]-'a']; 
            temp->cw++;
        }

        temp->ew++ ;
        
    }

    int countwords (string word ){
        Node * temp = root ; 
        int n = word.size() ; 

        for(int i = 0 ; i< n ; i++ ){

            if(temp->present(word[i]) == false ){
                return  0 ; 
            }

            temp= temp->links[word[i] - 'a']; 
        }

        return temp->ew ;
        
    }

    int countprefix(string word ){
        Node * temp = root ; 
        int n = word.size() ; 

        for(int i = 0 ; i< n ; i++ ){

            if(temp->present(word[i]) == false ){
                return  0 ; 
            }

            temp= temp->links[word[i]-'a']; 
        }

        return temp->cw ;

    }

    void erase(string word ){

        Node * temp = root ; 
        int n = word.size() ; 

        for(int i = 0 ; i< n ; i++ ){
            temp= temp->links[word[i]-'a']; 
            temp->cw--;
        }

        temp->ew-- ;

    }
};
int main(){

    Trie t ; 

    t.insert("samsung"); 
    t.insert("samsung");
    t.insert("samsung");
    t.insert("samsung");

    t.insert("vivo"); 
    t.insert("vivo");
    cout<<"Hello "<<endl ;
    cout<<t.countwords("samsung")<<endl; 
    t.erase("samsung");
    cout<<t.countwords("samsung")<<endl; 
    
    cout<<t.countprefix("vivo")<<endl; 

    
}