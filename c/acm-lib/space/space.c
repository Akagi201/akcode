#include <stdio.h>

int main(void)
{
    int i;
    int a[7] = {1, 2, 3, 4, 5, 6, 7};

    /* 数据两两之间有空格，两头没有空格的写法 */
    for(i=0; i<7; ++i)
	{
        if(i)
        {
            printf(" ");
        }
        printf("%d", a[i]);
    }
    printf("\n");
    return 0;
}
