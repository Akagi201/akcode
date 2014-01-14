
#include <stdio.h>

#define VERSION "abcdefg"
#define USER_AGENT "User-Agent: Linux-DUC/"VERSION

int main(void)
{
    printf(USER_AGENT);
    return 0;
}
