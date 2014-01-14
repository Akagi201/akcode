
#include <stdio.h>

static int delete_spaces(const char *src, char *dst, int dstlen)
{
	char c;
	int i = 0;

	if ((NULL == src) || (NULL == dst) || (dstlen < 0))
	{
		return -1;
	}

	while ((c = *src++) != '\0')
	{
		if (c != ' ')
		{
			//dst[i++] = c;
            *dst++ = c;
            ++i;
			if (i >= dstlen)
			{
				return -1;
			}
		}
	}
	*dst = '\0';
	return 0;
}

int main(void)
{
    char src[] = "a b";
    char dst[10] = {0};
    delete_spaces(src, dst, 10);
    printf("dst: %s\n", dst);
    return 0;
}
