
#include <stdio.h>
#include <string.h>

#define A "wwww.baidu.com"

int main(void)
{
    int a = sizeof(A);
    int b = strlen(A);
    printf("%d, %d\n", a, b);

    return 0;
}
