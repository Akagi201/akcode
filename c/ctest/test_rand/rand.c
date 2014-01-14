#include <stdio.h>
#include <stdlib.h>

int main()
{
    int i;
    srand((unsigned)time(NULL));
    i = rand();
    printf("%d\n", i);
    return 0;
}

