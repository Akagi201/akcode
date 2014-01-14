#include <stdio.h>
#include <stdlib.h>

#define I64(C) C##LL

int main(void)
{
    unsigned long long ll_mac = I64(0xffffffffffff);
    printf("### %llu.\n", ll_mac);
    return 0;
}



