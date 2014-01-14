
#include <stdio.h>

int f(int *p)
{
    if (NULL == p)
    {
        return -1;
    }
    return 2;
}

int main(void)
{
    int *p = NULL;
    int ret = 0;
    //int a = 1;
    //p = &a;
    //int ret = 0;
    ret = f(p);
    printf("%d\n", ret);

    return 0;
}

