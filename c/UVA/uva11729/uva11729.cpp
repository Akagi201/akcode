
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>

using namespace std;

typedef struct{
    int wait;
    int work;
}NODE;

int cmp(NODE x, NODE y)
{
    if (x.work > y.work) 
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int main(void)
{
    int n = 0;
    int cas = 1;
    int i = 0;
    int t = 0;
    int ans = 0;
    int sum = 0;
    NODE war[1004] = {0};

#ifndef ONLINE_JUDGE
    freopen("input.txt", "rt", stdin);
    freopen("output.txt", "wt", stdout);
#endif

    while (EOF != scanf("%d", &n), n) {
        sum = 0;
        for (i = 0; i < n; ++i) {
            scanf("%d%d", &war[i].wait, &war[i].work);
        }
        sort(war, war + n, cmp);
        for (i = 0; i < n; ++i) {
            sum += war[i].wait;
            ans = max(ans, sum + war[i].work);
        }
        printf("Case %d: %d\n", cas++, ans);
    }

    return 0; 
}
