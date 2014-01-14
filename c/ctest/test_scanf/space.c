#include <stdio.h>

int main(void)
{
    int a,b;
    scanf("%d - %d", &a, &b); // 输入1-2 1 - 2 1- 2 1 -2全对
    scanf("%d-%d", &a, &b); // 只有1-2可以
    printf("%d, %d\n", a, b);
    return 0;
}
