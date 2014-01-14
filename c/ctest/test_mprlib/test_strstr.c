
/** @file test_strstr.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief
  *
  * @author   liuboyf1
  * @date     2012-12-26
  * @version  V1.0.0
  *
  * @note ///Description here
  * @note History:
  * @note     <author>   <time>    <version >   <desc>
  * @note
  * @warning
  */

#include <stdio.h>

char *strstr(const char *str, const char *subString)
{
    char *cp, *s1, *s2;

    if (NULL == str)
    {
        return NULL;
    }

    if (subString == 0 || *subString == '\0')
    {
        return (char *) str;
    }

    for (cp = (char *) str; *cp; cp++)
    {
        s1 = cp;
        s2 = (char *) subString;

        while (*s1 && *s2 && (*s1 == *s2))
        {
            s1++;
            s2++;
        }

        if (*s2 == '\0')
        {
            return cp;
        }
    }

    return 0;
}

char *mprStrnstr(const char *str, const char *pattern, int len)
{
    cchar   *start, *p;
    int     i;

    if (str == 0 || pattern == 0 || len == 0) {
        return 0;
    }

    while (*str && len-- > 0) {
        if (*str++ == *pattern) {
            start = str - 1;
            for (p = pattern + 1, i = len; *p && *str && i >= 0; i--, p++) {
                if (*p != *str++) {
                    break;
                }
            }
            if (*p == '\0') {
                return (char*) start;
            }
        }
    }
    return 0;
}

int main(void)
{
	char *p_user_name = "";
	char *p_passwd = "";
	char *url_query = "http://172.8.5.111/Streaming/channels/1/preview/?username=admin&password=12345 ";
	//p_user_name = strstr(url_query, "username=");
	//p_passwd = strstr(url_query, "password=");
	p_user_name = mprStrnstr(url_query, "username=");
	p_passwd = mprStrnstr(url_query, "password=");
	printf("p_user_name = %s\n", p_user_name);
	printf("p_passwd = %s\n", p_passwd);

    return 0;
}

