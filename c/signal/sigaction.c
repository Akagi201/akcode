/*
 * @file sigaction.c
 * @author Akagi201
 * @date 2014/10/29
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

/*自定义信号处理函数*/
void my_func(int sign_no) {
    if (sign_no == SIGINT) {
        printf("I have get SIGINT\n");
    } else if (sign_no == SIGQUIT) {
        printf("I have get SIGQUIT\n");
    }
}

int main() {
    struct sigaction action;/*定义sigaction结构体*/
    printf("Waiting for signal SIGINT or SIGQUIT...\n");

    /*sigaction结构体初始化*/
    action.sa_handler = my_func;/*指定信号处理函数*/
    sigemptyset(&action.sa_mask);/*将参数sa_mask信号集初始化并清空*/
    action.sa_flags = 0; /*这里的0就相当于SA_NOMASK*/ // try SA_RESETHAND
    /*发出相应的信号并跳转到信号处理函数*/
    sigaction(SIGINT, &action, 0);
    sigaction(SIGQUIT, &action, 0);
    printf("The first pause\n");
    pause();
    printf("The second pause\n");
    pause();
    exit(0);
}
