#include <stdio.h>

#define ERR_PRINT(str) do{}while (0)
#define ERR_PRINT(fmt, args...) do{}while (0)

int main()
{
    char s[] = "abcdefghijhk";
    ERR_PRINT(s);
    ERR_PRINT(s,1);
    return 0;
}
