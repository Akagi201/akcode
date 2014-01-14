#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <error.h>
#include <stdlib.h>

int main()
{
    int fd, n;
    printf("Please select a way of creating files:\n");
    printf("1.creat 2.open\n");
    scanf("%d", &n);
    if (1 == n)
    {
        if (-1 == (fd = creat("hello.txt", S_IRWXU)))
        {
            perror("creat");
            exit(1);
        }
        else
        {
            printf("Create file success\n");
        }
    }
    else if (2 == n)
    {
        if (-1 == (fd = open("hello.txt", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR)))
        {
            perror("open");
            exit(1);
        }
        else
        {
            printf("create file success \n");
        }
    }
    else
    {
        printf("your choice may be wrong \n");
    }
    close(fd);
    return 0;
}





























