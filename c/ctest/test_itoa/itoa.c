#include <stdio.h>
#include <stdlib.h>
// CE

int main(void)
{
    long number = 12345;
    char string[25];
    itoa(number, string, 2/* 这里是进位制, 可以使任意的,此处为2进制*/);
    printf("integer = %d string = %s\n", number, string);
    return 0;
}

