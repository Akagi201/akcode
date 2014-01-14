#include <stdio.h>

int main()
{
    long n3 = 2000000000;
    long n4 = 1234567890;
    printf("%ld, %ld\n", n3, n4);
    printf("%lld, %lld\n", n3, n4);
    printf("%Ld, %Ld\n", n3, n4);
    return 0;
}
