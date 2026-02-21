# include<bits/stdc++.h>
using namespace std ; 

typedef vector<vector <int > > vvi ;
typedef vector<int> vi ; 

#define  print(vector) for (auto it: vector) cout<<it<<" " ; 


const int N= 2e5+1 ; 
int vis[N] ;
int dis[N] ;

int solve(int start , vector<vector<int> > &tree){
    vis[start]= 1 ;
    dis[start] = 0 ; 

    int diameter=0;

    queue<int> q ; 
    q.push(start);

    while(!q.empty()){
        int node = q.front() ; 
        q.pop();

        for (auto newnode : tree[node]){
            if (!vis[newnode]){
                q.push(newnode);
                vis[newnode]=1;

                dis[newnode] = 1+ dis[node];
                diameter= max(diameter , dis[newnode]);
            }
        }
    }

    return diameter;
}

int main(){
    int n ;
    cin>>n ; 
    vector <vector <int> > tree(n+1);

    for(int i = 1 ; i< n ; i++ ){
        int a , b ; 
        cin>>a>> b ; 
        tree[a].push_back(b);
        tree[b].push_back(a);
    }

    int vis1[n+1] ={0};

    // Farthest node from 1 ;
    vis1[1]= 1 ;
    int ans = 1 ;

    queue<int> q ; 
    q.push(1);
    while(!q.empty()){
        int node = q.front() ; 
        q.pop();
        ans =node; 

        for (auto newnode : tree[node]){
            if (!vis1[newnode]){
                q.push(newnode);
                vis1[newnode]=1;
            }
        }
    }

   

    cout<<solve(ans , tree )<<endl;




    
}