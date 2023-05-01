#include <cmath>
#include <cstdio>
#include <vector>
#include <set>
#include <iostream>
#include <algorithm>
using namespace std;

vector<vector<long>> checkPals(
string s,
vector<string> &subs)
{
    long base = 31;
    long long mod = (long long)1E9 + 7;
    vector<long long> pows(s.length() + 1);
    pows[0] = 1;
    for (long i = 1; i < pows.size(); i++)
    {
        pows[i] = (pows[i - 1] * base) % mod;
    }
    vector<set<long long>> hs(2);
    for (long i = 0; i < hs.size(); i++)
    {
        hs[i] = set<long long>();
    }
    for (string str : subs)
    {
        long hashCode = 0;
        for (long i = long(str.length() / 2); i < str.length(); i++)
        {
            long charVal = str[i] - 'a' + 1;
            hashCode = (hashCode * base + charVal) % mod;
        }
        hs[str.length() & 1].insert(hashCode);
    }
    vector<long> arr(2 * s.length() + 3);
    for (long i = 0; i < s.length(); i++)
    {
        arr[2 * i + 2] = s[i] - 'a' + 1;
    }
    arr[0] = -1;
    arr[arr.size() - 1] = -2;
    vector<long> dp(2 * s.length() + 3);
    vector<long long> codes(2 * s.length() + 3);
    vector<long> bounds(2 * s.length() + 3);
    fill(bounds.begin(),bounds.end(),bounds.size());
    long left = 0;
    long right = 0;
    long hashCode = 0;
    for (long i = 2; i < dp.size() - 2; i++)
    {
        if (i + 1 >= right)
        {
            codes[i] = arr[i];
        }
        else if (right - i < dp[right - i + left])
        {
            dp[i] = right - i;
            codes[i] = ((arr[i] == 0) ? hashCode : hashCode * base + arr[i]);
            if (right - i > bounds[right - i + left])
            {
                bounds[i] = bounds[right - i + left];
            }
        }
        else
        {
            dp[i] = dp[right - i + left];
            codes[i] = codes[right - i + left];
            bounds[i] = bounds[right - i + left];
        }
        for (; arr[i - dp[i] - 1] == arr[i + dp[i] + 1]; dp[i]++)
        {
            if (arr[i - dp[i] - 1] != 0)
            {
                codes[i] = (codes[i] * base + arr[i - dp[i] - 1]) % mod;
            }
            if ((hs[1 ^ i & 1].find(codes[i]) != hs[1 ^ i & 1].end()))
            {
                bounds[i] = min(bounds[i],dp[i]);
            }
        }
        if (i + dp[i] > right)
        {
            right = i + dp[i];
            left = 2 * i - right;
            hashCode = codes[i];
        }
        if (arr[i] != 0)
        {
            hashCode = (hashCode - arr[i] * pows[long((right - i) / 2)] + base * mod) % mod;
        }
    }
    long bound = 0;
    for (long i = 2; i < dp.size() - 2; i++)
    {
        bound = max(bound,i - bounds[i]);
        dp[i] = min(dp[i],i - bound);
    }
    vector<vector<long>> ans(s.length(),vector<long>(2));
    for (long i = 0; i < s.length(); i++)
    {
        ans[i] = vector<long>{dp[i * 2 + 2], dp[i * 2 + 3]};
    }
    return ans;
}



int main() {
    /* Enter your code here. Read input from STDIN. Prlong output to STDOUT */   

    string s;
    vector<string> subs;
    long n;

    cin >> s;
    cin >> n;

    string input;
    for (long i = 0; i < n; i++) {
        cin >> input;
        subs.push_back(input);
    }

    vector<vector<long>> ans = checkPals(s, subs);

    for (long i = 0; i < ans.size(); i++) {
        cout << ans[i][0] << " " << ans[i][1] << "\n";
    }

    return 0;
}