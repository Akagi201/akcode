// fwrite1.c

#include <stdio.h>

struct mystruct
{
    int i;
    char ch;
};

int main(void)
{
    FILE *stream;
    struct mystruct s;
    if (NULL == (stream = fopen("a.txt", "wb")))
    {
        fprintf(stderr, "Cannot open output file.\n");
        return 1;
    }
    s.i = 0;
    s.ch = 'A';
    fwrite(&s, sizeof(s), 2, stream); // 写入的struct文件
    fclose(stream);  // 关闭文件

    return 0;
}
