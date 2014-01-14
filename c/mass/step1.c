// for to while
#include <stdio.h>
// 找出素数
void primes(int cap)
{
    int i, j, composite, t = 0;

    while (t < cap * cap)
    {
        i = t / cap;
        j = t++ % cap;
        if (i <= 1)
        {
        }
        else if (!j)
        {
            composite = j;
        }
        else if (j == i && !composite)
        {
            printf("%d\t", i);
        }
        else if (j > 1 && j < i)
        {
            composite += !(i % j);
        }
    }

    printf("\n");
}

int main()
{
    primes(100);
    return 0;
}
