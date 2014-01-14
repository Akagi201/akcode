
#include <stdio.h>

int main(void)
{
    int i = 0;
    for (i = -15; i < 15; ++i)
    {
        printf("%d mod 10 = %d\n", i, i % 10);
    }

    return 0;
}


