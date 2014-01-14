#include <stdio.h>

int main(void)
{
    char a[16] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99};
    int *p;
    printf("########### a = %p.\n", a);
    p = &a[1];
    printf("########### p = %p.\n", p);
    *p = 1;
    printf("########### *p = %d.\n", *p);
    return 0;
}
