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
 
//FOR DEBUGGING
#define printarr(arr) for(int i=0;i<(sizeof(arr)/sizeof(arr[0]));i++){ cout << arr[i] << " ";}cout << endl;
#define printvec(v) for(int i=0;i<v.size();i++){ cout << v[i] << " ";}cout << endl;
#define printset(s) for(auto i : s){cout << i << " ";}cout<<endl;
 
//____________________________________________________________________//
 
 
void solve(){
    
    string s ; 
    cin>>s; 

    int n = s.size() ; 

    char curr = s[0];
    int count = 0 ; 

    vector< pair<char , int >> cnt; 

    for (int i = 0 ; i< n ; i++ ){

        if(s[i] == curr ){
            count++ ; 
        }

        else{

            cnt.push_back({curr , count});

            curr = s[i] ; 
            count =1 ; 
        }
    }

    cnt.push_back({curr,count});

    int prev= cnt[0].second;


    int ans = 0 ; 
    if (prev >= 3 ){
        cnt[0].second = 2 ;
        ans+=prev-2;
        prev = 2 ;
    }


   
    for(int i = 1 ; i< cnt.size() ; i++ ){

        int curr = cnt[i].second;
        if (prev == 2){
            if (curr >=2 ){
                cnt[i].second = 1;
                ans += curr -1 ; 
            }
        }

        else {

            if (curr >=3 ){
                cnt[i].second = 2;
                ans += curr -2 ; 
            }

        }

        prev = cnt[i].second  ; 
    }

    cout<<endl;

   


 

    string x =""; 
    int i=  0 ; 


    while( i<cnt.size() ){
        for (int j = 0 ; j< cnt[i].second ; j++ ){
            x+=cnt[i].first;
        }

        i++;
    }

    cout<<x<<endl;
    
}
 
signed main(){
 
    //do-not-use this in case of interactive
    ios_base::sync_with_stdio(0); //
    cin.tie(0);                   //
    //   //   //  //  //  //  //  //  
 
    solve() ; 
 
}