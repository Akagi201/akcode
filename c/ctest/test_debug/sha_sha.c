
#include <stdio.h>
#include <limits.h>

#define STR(s) #s   //> #把宏参数变成一个字符串
#define CONS(a, b) (int)(a##e##b)  //> ##把两个宏参数贴合在一起

int main(void)
{
    printf(STR(vck));           // 输出字符串"vck"
    printf("\n%d\n", CONS(2, 3)); // 2e3 输出:2000
    printf("%d\n", (int)(2e3));
    return 0;
}
