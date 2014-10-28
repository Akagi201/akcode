/*
 * @file fifo_read.c
 * @author Akagi201
 * @date 2014/10/28
 *
 * 读管道程序, 先运行
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#define MYFIFO    "/tmp/myfifo"  /*有名管道文件名*/

/*在limits.h中有 #define PIPE_BUF  4096 即4个字节大小*/
#define MAX_BUFFER_SIZE        PIPE_BUF /*定义在limits.h中*/

int main() {
    char buff[MAX_BUFFER_SIZE] = {0};
    int fd = -1;
    int nread = 0;

    /*判断有名管道是否已经存在，若尚未创建，则以相应的权限创建*/
    if (access(MYFIFO, F_OK) == -1) {
        if ((mkfifo(MYFIFO, 0666) < 0) && (errno != EEXIST)) {
            printf("Cannot create fifo file\n");
            exit(1);
        }
    }

    /*以只读阻塞方式打开有名管道*/
    fd = open(MYFIFO, O_RDONLY);
    if (fd == -1) {
        printf("Open fifo file error\n");
        exit(1);
    }
    while (1) {
        memset(buff, 0, sizeof(buff));
        if ((nread = read(fd, buff, MAX_BUFFER_SIZE)) > 0) {
            printf("Read '%s' from FIFO\n", buff);
        }
    }
    close(fd);
    exit(0);

}/*end*/
