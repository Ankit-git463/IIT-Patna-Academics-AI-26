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
 
static bool comp (pair<int ,int> &a , pair<int, int> &b){
    if(a.second <= b.second ) {
        return a.first >= b.first ;
    } 
    else {
        return a.first <= b.first;
    }

   
}
void solve(){
    
    int n ; cin>> n ; 

    vector < pair<int, int> > v(9); 

    for(int i = 0 ; i< 9 ; i++ ){
        cin>>v[i].second;
        v[i].first= i+1; 
    }

    int mi = INT_MAX ; 
    int d = 0 ; 

    for(auto p : v) {
        if(mi>= p.second){
            mi = p.second; 
            d= p.first ; 
        }
    }


    // Not Possible case 
    if(n / mi  ==  0 ){
        cout<<-1<<endl ;
        return ; 
    }

    // number of max length with minimum paint
    string ans = ""; 

    int sz = n/mi;
    int len = n/mi ; 

    while(len-- ){
        ans+=('0'+d);
    } 

    int left = n- mi*sz ;
   

    // do swappings now

    for(int i = 0 ; i<9 ; i++ ){
        v[i].second = v[i].second-mi;
    } 
    reverse(v.begin() , v.end()); 

    int end = 0; 

    for(int i = 0 ; i< 9 ; i++ ){

        int canbe =0; 
        if(v[i].second == 0 ){
            end = i ; 
            break;
        }

        // using the leftout paint 
        canbe = left/v[i].second ; 
        left = left - canbe*v[i].second ;
        v[i].second = canbe ; 
        
    }

    int ind = 0 ; 
    for(int i = 0 ; i< end ; i++ ){
        while(v[i].second > 0 ){
            ans[ind] ='0'+ 9-i; 
            ind++;
            v[i].second--; 
        }

    }

    cout<<ans<<endl; 






    cout<<endl;



 
}
 
signed main(){
 
    //do-not-use this in case of interactive
    ios_base::sync_with_stdio(0); //
    cin.tie(0);                   //
    //   //   //  //  //  //  //  //  
 
    solve(); 
 
}