
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// calculate the offset of t in S
#define offsetof(S,t)     (size_t)&(((S *)0)->t)

typedef struct _S1{
    char a;
    char b;
    double c;
}S1;

//__attribute__((packed)) means no alignment optimization
typedef struct _S2{
    char a;
    char b;
    double c;
}__attribute__((packed)) S2;

typedef struct _Y
{
    int a;
    int b;
    char c;
    char content[0];
} Y;

#pragma pack(push, 4)
struct a_4
{
    short v1;
    int v2;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct a_1
{
    short v1;
    int v2;
};
#pragma pack(pop)

int main()
{
    printf("############ sizeof(double) = %d.\n", sizeof(double));

    //the effect of __attribute__((packed))，使用了__attribute__((packed))后，没有padding了。
    printf("\nsizeof(S1)=%u, offsetof(S1,c)=%u\n", sizeof(S1),offsetof(S1,c));
    printf("sizeof(S2)=%u, offsetof(S2,c)=%u\n", sizeof(S2),offsetof(S2,c));
    //这个结构很有趣，这个结构Y是没有优化的，所以编译器给这个结构的最后增加了部分的padding 。

    //所以这个结构体的大小为12，但是content的偏移却是9
    printf("sizeof(Y)=%u, offsetof(Y, content)=%u, offsetof(Y, c)=%u\n", sizeof(Y), offsetof(Y, content), offsetof(Y, c));

    // one example of #pragma pack() directives
    printf("sizeof(a_1)=%u \n", sizeof(struct a_1));
    printf("sizeof(a_4)=%u \n", sizeof(struct a_4));
    return 0;
}
