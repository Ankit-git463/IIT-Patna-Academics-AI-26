# include<bits/stdc++.h>
using namespace std ; 

typedef vector<vector <int > > vvi ;
typedef vector<int> vi ; 

#define  print(vector) for (auto it: vector) cout<<it<<" " ; 
vector < vector <int> > uptree ; 
vector <int> depth;
vector <vector <int> > adj;

void bfs(int root , vector < vector <int> >&adj){
    queue<int> q ; 

    q.push(root); 
    while(!q.empty()){
        int node = q.front();
        q.pop();

        for (auto newnode : adj[node]){
            if( depth[newnode] == 0 and newnode!=root ){
                depth[newnode] = 1+ depth[node];
                q.push(newnode);
                uptree[newnode][0] = node ; 
            }
        }

    }
}


int dis(int x,int y ){

    int hx = depth[x];
    int hy = depth[y]; 

    if (hx<hy){
        swap(hx , hy );
        swap(x,y) ; 
    }
    int diff = hx-hy; 

    // make the depth equal 
    for (int i = 0 ; i < 20 ; i++ ){
        if (diff&(1<< i )){
            x= uptree[x][i];

            if (x== -1 )break ; 
        }
    }

    if (x== y )return hx-hy;
   

    // Now both `x` and `y` have the same parent (LCA is their parent)
    return depth[x] + depth[y];
     
}

int main(){
    int n  ,q ; 
    cin>>n >>q  ; 

    uptree.assign(n+1 , vector <int > (20 , -1 ));
    depth.assign(n+1 , 0 );
    vector  <int> x;
    adj.assign(n+1, x );

    for (int i = 1 ; i< n ; i++ ){
        int x , y ; 
        cin>>x >>y ; 
        adj[x].push_back(y); 
        adj[y].push_back(x); 
    }


    bfs(1,adj);

    // make uptree 
    for(int i = 1 ; i< n ; i++ ){
        for  (int j = 1 ; j<=n ; j++ ){
            if (uptree[i][j-1] != -1 ){
                uptree[i][j] = uptree[uptree[i][j-1]][j-1];
            }
        }
    }
    for (auto v: uptree){
        print(v);
        cout<<endl;
    }
    

    

    while(q--) { 

        int x , y ; 
        cin>>x>>y ; 
        cout<<x<<" "<<y <<" -> ";
        cout<<dis(x,y )<<endl; 

        

    }

    
}