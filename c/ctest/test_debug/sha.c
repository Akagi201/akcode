#include <stdio.h>
#include <limits.h>

#define TOW (2)
#define MUL(a, b) (a*b)
#define STR(s) #s    //> #把宏参数变成一个字符串
#define CONS(a, b) (int)(a##e##b) //> ##把2个宏参数贴合在一起

// 解决宏参数不能展开问题，多加一层中间转换宏就可以了，加这层宏的用意是把所有
// 宏的参数在这层里全部展开，那么在转换宏利的那一个宏(_STR1)就能得到正确的宏参数

#define _STR1(s) #s
#define STR1(s) _STR1(s)    //> 转换宏
#define _CONS1(a, b) (int)(a##e##b)
#define CONS1(a, b) _CONS1(a, b)


int main()
{
    printf("%d*%d=%d\n", TOW, TOW, MUL(TOW,TOW));
    printf("%d*%d=%d\n", (2), (2), ((2)*(2)));
    // INT_MAX TOW不会被展开,当宏参数是另一个宏的时候，凡是宏定义里有用#或##
    // 的地方宏参数使不会再展开的
    printf("int max: %s\n", STR(INT_MAX));   // INT_MAX #include <limits.h>
    printf("int max: %s\n", "INT_MAX"); 
    //printf("%d\n", CONS(TOW, TOW)); // compile error
    // printf("%d\n", (int)(TOW e TOW));
    
    // 加转换宏之后
    printf("int max: %d\n", STR1(INT_MAX));
    printf("%d\n", CONS1(2, 2));
    printf("%d\n", _CONS1(2, 2));

    return 0;
}
