/*
 * @file fifo_write.c
 * @author Akagi201
 * @date 2014/10/28
 *
 * 写管道程序
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#define MYFIFO    "/tmp/myfifo"  /*有名管道文件名*/

/*在limits.h中有 #define PIPE_BUF  4096 即4个字节大小*/
#define MAX_BUFFER_SIZE        PIPE_BUF /*定义在limits.h中*/

/*参数为即将写入的字符串*/
int main(int argc, char *argv[]) {
    int fd = -1;
    char buff[MAX_BUFFER_SIZE] = {0};
    int nwrite = 0;

    if (argc <= 1) {
        printf("Usage: ./fifo_write string\n");
        exit(1);
    }

    /*sscanf()表示从字符串中格式化输出，与scanf类似，都是用于输入的，只是
    scanf()以键盘为输入源，sscanf()是以固定字符串为输入源*/
    sscanf(argv[1], "%s", buff);/*将argv[1]的内容以字符串(%s)的形式存入buf中*/

    /*以只写阻塞方式打开FIFO管道*/
    fd = open(MYFIFO, O_WRONLY);
    if (fd == -1) {
        printf("Open fifo file error\n");
        exit(1);
    }
    /*向管道中写入字符串*/
    if ((nwrite = write(fd, buff, MAX_BUFFER_SIZE)) > 0) {
        printf("Write '%s' to FIFO\n", buff);
    }
    close(fd);
    exit(0);
} /*end*/
			
