#include <bits/stdc++.h>
#define int  long long
#define test int t;cin>>t;while(t--)
#define yes cout<<"YES"<<endl
#define no cout<<"NO"<<endl
using namespace std;
 
const int M=1e9+7;
 


void solve(){

    int n, i, j, k, l ,t, ct = 0, y, q, m, x, mx, mx2, num, temp, val, maxi;
	cin>>n;

    int a[n];

	
	for(i=0;i<n;i++)
	{
		cin>>a[i];
	}
	
	cin>>m;

    int b[m];
	
	for(i=0;i<m;i++)
	{
		cin>>b[i];
	}
	
	sort(a,a+n);
	sort(b,b+m);
	
	maxi = -1000000000;
	
	for(i=0;i<n;i++)
	{
		val = a[i] - 1;
		x = 3*(n - i) + 2*i;
		
		num = upper_bound(b,b+m,val) - b;
		y = 3*(m - num) + 2*num;
		
		temp = x -  y;
		
		if(temp > maxi)
		{
			maxi = x - y;
			mx = x;
			mx2 = y;
		}
		
		else if((temp == maxi) && (x > mx))
		{
			mx = x;
			mx2 = y;
		}
	}
	
	x = 2*n;
	y = 2*m;
 
	temp = x - y;
	
	if(temp > maxi)
	{
		maxi = x - y;
		mx = x;
		mx2 = y;
	}
	
	else if((temp == maxi) && (x > mx))
	{
		mx = x;
		mx2 = y;
	}
			
	cout<<mx<<":"<<mx2<<"\n";
	
	
    
}
 
signed main(){
 
    //do-not-use this in case of interactive
    ios_base::sync_with_stdio(0); //
    cin.tie(0);                   //
    //   //   //  //  //  //  //  //  
 
    solve();
 
}