#include <stdio.h>

int main(void)
{
    int *pint;
    int a = 1;
    pint = &a;
    void *pvoid;
    pvoid = pint;
    printf("*pint = %d\n", 1);
    printf("*pvoid = %d\n", (int)*((int *)pvoid));
    return 0;
}
