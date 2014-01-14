
#include <stdio.h>

int main()
{
    int t;
    int i;
    int a;
    int b;
    int n;
    int ans;

    scanf("%d", &t);
    for (i = 0; i < t; ++i)
    {
        scanf("%d%d%d", &a, &b, &n);
        if (1 == n)
        {
            ans = a;
        }
        else if (2 == n)
        {
            ans = b;
        }
        else
        {

        }
    }

    return 0;
}
