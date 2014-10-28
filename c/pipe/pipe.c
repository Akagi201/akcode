
/*
 * @file pipe.c
 * @author Akagi201
 * @date 2014/10/28
 *
 *
 */

#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATA_LEN    (256)
#define DELAY_TIME    (1)

int main() {
    pid_t pid = -1;
    int pipe_fd[2];
    char buf[MAX_DATA_LEN] = {0};
    const char data[] = "Pipe Test Program";
    int real_read = 0;
    int real_write = 0;

    memset((void *) buf, 0, sizeof(buf));

    /*创建管道*/
    if (pipe(pipe_fd) < 0) {
        printf("Pipe create error\n");
        exit(1);
    }

    /*创建一个子进程*/
    if ((pid = fork()) == 0) {
        /*子进程关闭写描述符，并通过使子进程暂停3s等待父进程已关闭相应的读描述符*/
        close(pipe_fd[1]);
        sleep(DELAY_TIME * 3);
        /*子进程读取管道内容*/
        if ((real_read = read(pipe_fd[0], buf, MAX_DATA_LEN)) > 0) {
            printf("I am child progress.I have read %d bytes from the pipe: '%s'\n", real_read, buf);
        }
        close(pipe_fd[0]); /*关闭子进程读描述符*/
        exit(0);
    }
    else if (pid > 0) {
        /*父进程关闭读描述符，并通过使父进程暂停1s等待子进程已关闭相应的写描述符*/
        close(pipe_fd[0]);
        sleep(DELAY_TIME);
        if ((real_write = write(pipe_fd[1], data, strlen(data))) != -1) {
            printf("I am parent progress.I have wrote %d bytes to the pipe:'%s'\n", real_write, data);
        }
        close(pipe_fd[1]);  /*关闭父进程写描述符*/
        waitpid(pid, NULL, 0);  /*收集子进程退出信息，等待子进程退出*/
        exit(0);
    }
}
