
#include <stdio.h>
#include <string.h>

int main()
{
    char str[10] = "01:02:03";
    char *str1 = str + 3;
    int x =atoi(str1);
    printf("x = %d\n", x);
    return 0;
}
