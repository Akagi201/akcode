
#include <stdio.h>
#include <alloc.h>
#include <string.h>

int main(void)
{
    char *str;
    str = malloc(10);
    strcpy(str, "Hello");
    printf("String is %s\n Address is %p\n", str, str);
    return 0;
}


