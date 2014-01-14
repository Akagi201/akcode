// cfile.c

#include <stdio.h>
#include "cppexample.h"

extern int add(int x, int y);

int main(int argc, char *argv[])
{
    printf("%d\n", add(2, 3));
    return 0;
}

