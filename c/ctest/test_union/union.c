
#include <stdio.h>

typedef union demo
{
    char c;
    int a;
    int b;
}demo_t;

int main(int argc, char **argv)
{
    demo_t union_test;
    union_test.c = 'H';
    union_test.a = 10;
    union_test.b = 12;

    printf("size: %d\n", sizeof(union_test));
    printf("%c, %d, %d\n", union_test.c, union_test.a, union_test.b);

    return 0;
}
