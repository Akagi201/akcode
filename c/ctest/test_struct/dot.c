
#include <stdio.h>

typedef struct str
{
    int a;
    int b;
}Str;

int main()
{
    Str s = {a:1,b:2}; // 注意这里的冒号,gcc扩展

    /*
    Str s = 
    {
        .a = 1,
        .b = 2
    }; // 注意这里使用的点,C99规定方式
    */
    printf("a = %d, b = %d\n", s.a, s.b);
    return 0;
}
