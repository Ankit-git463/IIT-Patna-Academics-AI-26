#include<bits/stdc++.h>
 
using namespace std;
 
#define int long long 
 
vector<int> graph[100005];
int add[100005],sub[100005],val[100005];
 
int dfs(int i,int par){
	for(int j=0;j<graph[i].size();j++){
		int ind=graph[i][j];
		if(par!=ind){
			dfs(ind,i);
			add[i]=max(add[i],add[ind]);
			sub[i]=max(sub[i],sub[ind]);
		}
	}
	val[i]+=(add[i]-sub[i]);
	if(val[i]>0){
		sub[i]+=val[i];
	}
	else{
		add[i]-=val[i];
	}

    return 0;
}
 
signed main(){
	
	ios_base::sync_with_stdio(false);
    cin.tie(NULL);
	
	int n;
	cin >> n;
	for(int i=0;i<n-1;i++){
		int a,b;
		cin >> a >> b;
		graph[a].push_back(b);
		graph[b].push_back(a);
	}	
	for(int i=1;i<=n;i++){
		cin >> val[i];
	}
	dfs(1,-1);
	cout << sub[1]+add[1] << endl;
}
