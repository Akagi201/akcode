
#include <stdio.h>
#include <string.h>

typedef struct{
    unsigned int a;
    unsigned char b;
    unsigned short c;
}A;

#pragma pack(1)
typedef struct{
    unsigned int a;
    unsigned char b;
    unsigned short c;
}__attribute__ ((__packed__))
B;
#pragma pack()

int main()
{
    int i = 0;
    B bs;
    char aa[7] = {0};
    memset(&bs, 0, sizeof(bs));
    bs.a = 0x33112211;
    bs.b = 0x44;
    bs.c = 0x5533;
    printf("################# sizeof(A) = %d.\n", sizeof(A));
    printf("################# sizeof(B) = %d.\n", sizeof(B));
    memcpy(aa, &bs, 7);
    for (i = 0; i < 7; ++i)
    {
        printf("############### aa[%d] = 0x%x.\n", i, aa[i]);
    }

    return 0;
}

