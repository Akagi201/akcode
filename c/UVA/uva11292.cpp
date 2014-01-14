
// 思路: 尽可能用值小的骑士去砍头,从小的骑士开始选,贪心算法如下
#include <cstdio>
#include <algorithm>

using namespace std;

int const maxn = 20000 + 4;

int A[maxn], B[maxn];

int main()
{
    int n, m, i, cur, cost;
    while ((2 == scanf("%d%d", &n, &m)) && n && m)
    {
        for (i = 0; i < n; ++i)
        {
            scanf("%d", A + i);
        }

        for (i = 0; i < m; ++i)
        {
            scanf("%d", B + i);
        }

        sort(A, A + n);
        sort(B, B + m);

        cur = 0;  // 当前需要砍掉的头的编号
        cost = 0; // 当前总费用

        for (i = 0; i < m; ++i)
        {
            if (B[i] >= A[cur])
            {
                cost += B[i];   // 雇佣该骑士
                if (++cur == n)
                {
                    break;      // 如果头已经砍完, 及时退出循环
                }
            }
        }

        if (cur < n)
        {
            printf("Loowater is dommed!\n");
        }
        else
        {
            printf("%d\n", cost);
        }
    }
    return 0;
}
