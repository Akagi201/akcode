#include <stdio.h>

int checkCPUendian()
{
    union
    {
        unsigned int a;
        unsigned char b;
    }c;
    c.a = 1;
    return (1 == c.b);
} // return 0: big-endian else : little-endian

int main()
{
    if (0 == checkCPUendian())
    {
        printf("This CPU is big-endian!\n");
    }
    else
    {
        printf("This CPU is little-enidan!\n");
    }
    return 0;
}
