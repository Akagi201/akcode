#include <stdio.h>
#define LENGTH 100

int main()
{
    FILE *fd;
    char str[LENGTH];

    fd = fopen("hello.txt", "w+");  /* 创建并打开文件 */

    if (fd)
    {
        fputs("Hello World", fd); /* 写入字符串 */
        fclose(fd);
    }

    fd = fopen("hello.txt", "r");
    fgets(str, LENGTH, fd);
    printf("%s\n", str);
    fclose(fd);
    return 0;
}
