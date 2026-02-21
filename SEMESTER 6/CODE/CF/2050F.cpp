#include <bits/stdc++.h>
#define int  long long
#define test int t;cin>>t;while(t--)
#define yes cout<<"YES"<<endl
#define no cout<<"NO"<<endl
using namespace std;
 
const int M=1e9+7;
 
int nCr(int n, int r) {
    int sum=1;
     for(int i = 1; i <= r; i++){
            sum = sum * (n - r + i) / i;
        }
    return sum;
}
//O(log(min(a,b)))
int gcd(int a, int b){
    return b == 0 ? a : gcd(b, a % b);
}   
//O(log(min(a,b)))
int lcm(int a, int b){
    return (a / gcd(a, b)) * b;
}
 
//O(n)
int maxSubArraySum(int a[], int size)
{
    int max_so_far = INT_MIN, max_ending_here = 0;
 
    for (int i = 0; i < size; i++) {
        max_ending_here = max_ending_here + a[i];
        if (max_so_far < max_ending_here)
            max_so_far = max_ending_here;
 
        if (max_ending_here < 0)
            max_ending_here = 0;
    }
    return max_so_far;
}
 
int sqroot(int n){
    int l = 0, h = 1000000000,ans = n;
    while(l <= h){
        int mid = l + (h-l)/2;
        if( mid*mid <= n){
            ans = mid;
            l = mid + 1;
        }
        else
            h = mid - 1;
    }
    return ans;
}
 
 
bool isPowerOfTwo(unsigned int n){
return (n&&!(n&n-1));
}
 
int countSetBits(int n){
    int count=0;
    while(n){
        n=n&(n-1);
        count++;
    }
    return count;
}
 
void prime(int n){
    for(int i=0;i*i<n;i++){
        while(n%i == 0)
        {
            cout<<i<<" ";
            n=n/i;
        }
    }
    if(n>1) cout<<n<<" ";
}
 
bool isPrime(int n)
{
    if (n <= 1)
        return false;
    for (int i = 2; i <= n / 2; i++)
        if (n % i == 0)
            return false;
    return true;
}
 
int power(int a,int b){
    if(b==0) return 1;
    int tmp=power(a,b/2);
    int result=((tmp%M)*(tmp%M))%M;
    if(b%2==1) result =((result%M)*(a%M))%M;
    return result;
}
 
int mul(int a,int b){
return ((a%M)*(b%M)%M);
}
//___________________________________________________________________//
 

/// sparse table ///
vector < vector <int> > spt;
void build( vector <int> &arr ){
    int n = arr.size() ; 
    int k = log2(n+1) + 1 ; 
    spt.clear();
    spt.assign(n, vector<int>  (k, -1)) ; 

    // making the sparse table first column of the table 
    for(int i = 0 ; i< n ; i++ ){
        spt[i][0] = arr[i] ; 
    }

    // building the rest of the columns 
    for (int col  = 1 ; (1<<col ) <= n ; col++ ){

        for(int row = 0 ; row + (1<< col ) <= n ; row++ ){
            spt[row][col] = gcd(spt[row][col-1] , spt[row+ (1 << (col-1))][col -1 ]) ; 
        }
    }
}

int query(int l, int r ){
    int col = log2(r-l +1 )  ; 
    return gcd(spt[l][col], spt[r- (1<< col) +1 ][col] );
}

// a% m == b % m means |a-b| % m 
void solve(){
    int n , q ; 
    cin>>n >> q ; 

    vector <int> v(n); 
    for(int i = 0 ; i< n ; i++ )cin>>v[i] ; 
    vector <int> vv; 

    for (int i = 1 ; i< n ; i++ ){
        vv.push_back(abs(v[i-1] - v[i] )) ; 
    }

    build(vv); 


    while(q-- ){
        int l ,r; 
        cin>>l >> r ; 

        if(l== r ){
            cout<<0<<" " ; 
        }

        else{
            cout<<query(l-1 , r-2) << " ";
        }
    }

    cout<<endl;

    
    
}
 
signed main(){
 
    //do-not-use this in case of interactive
    ios_base::sync_with_stdio(0); //
    cin.tie(0);                   //
    //   //   //  //  //  //  //  //  
 
    test{
        solve();
    }
 
}