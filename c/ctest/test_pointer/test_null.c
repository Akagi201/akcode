
#include <stdio.h>
int main()
{
    int *p = NULL;
    printf("%p\n", p);
    printf("%lud\n", sizeof(p));
    int a = 10;
    p = &a;
    printf("%p\n", p);

    //unsigned int p_addr = (unsigned int)p;
    //printf("%s\n", p);
    //printf("%x", p_addr);
    return 0;
}
