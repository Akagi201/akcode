/*
 * @file sigset.c
 * @author Akagi201
 * @date 2014/10/29
 */

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

/*自定义的信号处理函数*/
void my_func(int signum) {
    printf("If you want to quit,please try SIGQUIT\n");
}

int main() {
    sigset_t set, pendset;/*定义信号集*/
    struct sigaction action1, action2;

    /*初始化信号集为空*/
    if (sigemptyset(&set) < 0) {                          /*void perror(const char *s);功能:用来将上一个*/
        perror("sigemptyset"); /*函数发生错误的原因输出标准设备(stderr)，参数s*/
        exit(1);              /*所指的字符串会先打印出，后面再加上错误原因字符串*/
    }                          /*此错误原因依照全局变量errno的值来决定要输出的字符串*/

    /*将相应的信号加入信号集*/
    if (sigaddset(&set, SIGQUIT) < 0) {
        perror("sigaddset");
        exit(1);
    }
    if (sigaddset(&set, SIGINT) < 0) {
        perror("sigaddset");
        exit(1);
    }
    /*查询SIGINT信号是否在信号集中*/
    if (sigismember(&set, SIGINT))/*成功返回1*/
    {
        /*sigaction结构初始化*/
        sigemptyset(&action1.sa_mask);
        action1.sa_handler = my_func;
        action1.sa_flags = 0;
        /*发出相应的信号，并跳转到信号处理函数处*/
        sigaction(SIGINT, &action1, NULL);
    }

    if (sigismember(&set, SIGQUIT)) {
        sigemptyset(&action2.sa_mask);
        action2.sa_handler = SIG_DFL;/*采用系统默认方式处理该信号*/
        action2.sa_flags = 0;
        sigaction(SIGQUIT, &action2, NULL);
    }

    /*设置信号屏蔽字，此时set中的信号不会被传递给进程，暂时进入待处理状态*/
    if (sigprocmask(SIG_BLOCK, &set, NULL) < 0) {
        perror("sigprocmask");
        exit(1);
    } else {
        printf("Signal set was blocked,Press any key!");
        getchar();  /*这就是输入任意键的关键，你不输入任意键，程序就一直停在这里*/
    }

    /*在信号屏蔽字中删除 set 中的信号*/
    if (sigprocmask(SIG_UNBLOCK, &set, NULL) < 0) {
        perror("sigprocmask");
        exit(1);
    } else {
        printf("Signal set is in unblock state\n");
    }

    while (1);
    exit(0);
}/*end*/


