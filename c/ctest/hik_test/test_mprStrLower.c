
#include <stdio.h>
#include <assert.h>

char *mprStrLower(char *str);

int main()
{
    char src[] = "Streaming/channels/102/preview";
    char *dst;
    dst = mprStrLower(src);
    printf("src = %s\n", src);
    printf("dst = %s\n", dst);
    return 0;
}

/*
 *  Map a string to lower case (overwrites original string)
 */
char *mprStrLower(char *str)
{
    char    *cp;

    assert(str != NULL);

    if (str == 0) {
        return 0;
    }

    for (cp = str; *cp; cp++) {
        if (isupper((int) *cp)) {
            *cp = (char) tolower((int) *cp);
        }
    }
    return str;
}

