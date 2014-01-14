// 把循环编程递归
#include <stdio.h>
// 找出素数
void primes(int cap, int t, int composite)
{
    int i, j;
    i = t / cap;
    j = t % cap;
    if (i <= 1)
    {
        primes(cap, t + 1, composite);
    }
    else if (!j)
    {
        primes(cap, t + 1, j);
    }
    else if (j == i && !composite)
    {
        (printf("%d\t", i), primes(cap, t + 1, composite));
    }
    else if (j > 1 && j < i)
    {
        primes(cap, t + 1, composite + !(i % j));
    }
    else if (t < cap * cap)
    {
        primes(cap, t + 1, composite);
    }
}

int main()
{
    primes(100, 0, 0);
    printf("\n");
    return 0;
}
