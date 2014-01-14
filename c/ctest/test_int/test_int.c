
#include <stdio.h>

int main()
{
    int a = 0xffffffff;
    if (a > 0)
    {
        printf("a > 0\n");
    }
    else
    {
        printf("a < 0\n");
    }

    printf("a = %d\n", a);
    return 0;
}
