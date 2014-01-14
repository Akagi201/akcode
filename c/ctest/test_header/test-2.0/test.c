// test.c

#include <stdio.h>
#include "test.h"

int i = 10;
char add1[] = "www.shellbox.cn!\n";
char add2[] = "www.scriptbox.cn\n";
extern void test1();
extern void test2();

int main()
{
    test1();
    printf("ok!\n");
    test2();
    printf("%d\n", i);
    return 0;
}
