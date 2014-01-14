// open.c

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    int fd, size;
    char s[] = "Linux Programmer!\n";
    char buffer[80];
    fd = open("/tmp/temp", O_WRONLY | O_CREAT);
    write(fd, s, sizeof(s));
    close(fd);
    fd = open("/tmp/temp", O_RDONLY);
    size = read(fd, buffer, sizeof(buffer));
    close(fd);
    printf("%s", buffer);
    return 0;
}
