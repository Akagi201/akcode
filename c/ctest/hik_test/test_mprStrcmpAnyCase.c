
#include <stdio.h>

int mprStrcmpAnyCase(char *str1, char *str2);

int main()
{
    char src[] = "/Streaming/channels/102/preview";
    char dst[] = "/streaming/channels/102/preview";
    printf("%d\n", mprStrcmpAnyCase(src, dst));
    return 0;
}

/*
 *  Case insensitive string comparison. Stop at the end of str1.
 */
int mprStrcmpAnyCase(char *str1, char *str2)
{
    int     rc;

    if (str1 == 0) {
        return -1;
    }
    if (str2 == 0) {
        return 1;
    }
    if (str1 == str2) {
        return 0;
    }
    for (rc = 0; *str1 && *str2 && rc == 0; str1++, str2++) {
        rc = tolower((int) *str1) - tolower((int) *str2);
    }
    if (rc) {
        return rc < 0 ? -1 : 1;
    } else if (*str1 == '\0' && *str2 == '\0') {
        return 0;
    } else if (*str1 == '\0') {
        return -1;
    } else if (*str2 == '\0') {
        return 1;
    }

    return 0;
}

