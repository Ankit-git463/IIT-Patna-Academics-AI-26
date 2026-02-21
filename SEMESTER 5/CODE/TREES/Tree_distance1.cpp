# include<bits/stdc++.h>
using namespace std ; 

typedef vector<vector <int > > vvi ;
typedef vector<int> vi ; 

#define  print(vector) for (auto it: vector) cout<<it<<" " ; 

const int N = (2*1e5)+1  ; 
int vis[N];
int dis[N];
int disb[N];

int bfs(vector < vector <int>> &tree  ,int start){
    vis[start] =  1 ;
    queue<int> q ; 
    q.push(start);
    int ans = start ; 
    while(!q.empty()){
        int node = q.front();
        ans = node; 
        q.pop();
        for (auto newnode : tree[node]){
            if(!vis[newnode]){
                q.push(newnode);
                vis[newnode]= 1 ;
            }
        }
    }
    return ans ; 
}

int bfs_dis(vector < vector <int>> &tree  ,int start ,int *dis){
    dis[start] =  0 ;
    vis[start] = 1 ; 
    queue<int> q ; 
    q.push(start);
    int ans = start ;
    
    while(!q.empty()){
        int node = q.front();
        q.pop();
        ans= node ; 
        for (auto newnode : tree[node]){
            
            if  (!vis[newnode]) {
                dis[newnode] = dis[node]+1 ; 
                q.push(newnode);
                vis[newnode] = 1 ;
            }
            
        }
    }
    return ans ; 
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

    for(int i = 0 ; i<=n ;i++ ){
        vis[i] =0 ;
        dis[i] =0; 
        disb[i] =0 ; 
    }

    // Finding the farthest node from 1 
    int a = bfs(tree , 1 );
    
    for(int i = 0 ; i<=n ;i++ ) vis[i] =0 ;
    // Finding the farthest node from a
    
    int b= bfs(tree , a );
    
    for(int i = 0 ; i<=n ;i++ ) vis[i] =0 ;
    bfs_dis(tree , b , disb);
    for(int i = 0 ; i<=n ;i++ ) vis[i] =0 ;
    bfs_dis(tree , a , dis);

    for(int i = 1 ; i<= n ;i++ ){
        cout<<max(dis[i] , disb[i]) <<" ";
    }

    cout<<endl;
















}