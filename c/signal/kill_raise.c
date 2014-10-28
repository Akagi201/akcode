/*
 * @file kill_raise.c
 * @author Akagi201
 * @date 2014/10/29
 *
 */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = 0;
    int ret = 0;
    /*创建一个子进程*/
    if ((pid = fork()) < 0) {
        /* 出错处理 */
        printf("Fork error\n");
        exit(1);
    }
    if (pid == 0) {
        /* 子进程 */
        /*在子进程中使用raise()函数发出SIGSTOP信号，使子进程暂停*/
        printf("I am child progress(pid:%d).I am waiting for any signal\n", getpid());
        raise(SIGSTOP);
        printf("I am child progress(pid:%d).I am killed by progress:%d\n", getpid(), getppid());
        exit(0);
    } else {
        /* 父进程 */
        sleep(2);  /*先让父进程休眠，让子进程执行，建议你把这句话去掉试一试*/
        /*在父进程中收集子进程发出的信号，并调用kill()函数进行相应的操作*/
        if ((waitpid(pid, NULL, WNOHANG)) == 0) { /*若pid指向的子进程没有退出，则返回0,且父进程不阻塞,继续执行下边的语句*/
            if ((ret = kill(pid, SIGKILL)) == 0) {
                printf("I am parent progress(pid:%d).I kill %d\n", getpid(), pid);
            }
        }
        waitpid(pid, NULL, 0);/*等待子进程退出，否则就一直阻塞*/
        exit(0);
    }
}/* end */
