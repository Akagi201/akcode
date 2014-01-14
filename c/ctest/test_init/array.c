
#include <stdio.h>

int main(void)
{
    int temp[10] = {[0 ... 9] = -2};
    int temp2[10] = {[0 ... 5] = -1};
    int temp3[10] = {[1 ... 5] = -3};

    int i = 0;

    for (i = 0; i < 10; ++i)
    {
        printf("%d\n", temp[i]);
        printf("%d\n", temp2[i]);
        printf("%d\n", temp3[i]);
    }
    return 0;
}

