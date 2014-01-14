#include <stdio.h>
#include <string.h>

int main()
{
    //char *dest = "hello, world";
    char a[100];
    char *dest = a;
    char src[100] = "hello, world";
    //char *src = "hello";
    puts(strcpy(dest, src));
    return 0;
}


