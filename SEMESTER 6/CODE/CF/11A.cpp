#include <bits/stdc++.h>
using namespace std;
int main()
{
    int n, i, j, c;
    vector<int> v;
    cin >> n;
    int arr[n][3];
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < 3; j++)
        {
            cin >> arr[i][j];
        }
    }
    for (i = 0; i < n; i++)
    {
        c = 0;
        for (j = 0; j < 3; j++)
        {
            if (arr[i][j] == 1)
            {
                c = c + 1;
            }
        }
        if (c >= 2)
        {
            v.push_back(1);
        }
    }
    int ans = 0;
    for (i = 0; i < v.size(); i++)
    {
        ans = v[i] + ans;
    }
    cout << ans;
    return 0;
}