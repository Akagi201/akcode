
/* fseek example 相当于 lseek, 定位文件指针*/
#include <stdio.h>

int main()
{
    FILE *pFile;
    pFile = fopen("myfile.txt", "w");
    fputs("This is an apple.", pFile);
    fseek(pFile, 9, SEEK_SET);
    fputs(" sam", pFile);
    fclose(pFile);
    return 0;
}
/* resault: in the file myfile.txt This is a sample. */
