#include <stdio.h>

int main()
{
    fprintf(stdout, "This is a standard output info!\n");
    fprintf(stderr, "This is a standard error info\n");
    return 0;
}

/*
 * ./log > output.log
 * ./log > output.log 2> error.log
 * ./log > output.log 2>&1
 */
