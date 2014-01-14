

#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>

using namespace std;

#define M (1000004)

long long a[M];
long long c[M];
long long tot;
long long m;
long long n;

int main(int argc, char *argv[])
{
    int i = 0;
    long long ans = 0;
#ifndef ONLINE_JUDGE
    freopen("input.txt", "rt", stdin);
    freopen("output.txt", "wt", stdout);
#endif
    ios::sync_with_stdio(false);
    while (cin >> n) {
        tot = 0;
        ans = 0;
        for (i = 1; i <= n; ++i) {
            cin >> a[i];
            tot += a[i];
        }
        m = tot / n;
        c[0] = 0;
        for (i = 1; i < n; ++i) {
            c[i] = c[i - 1] + m - a[i];
        }
        sort(c, c + n);
        for (i = 0; i < n; ++i) {
            ans += abs(c[n / 2] - c[i]);
        }
        cout << ans <<endl;
    }

    return 0; 
}
