/*
 * @file signal.c
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
    printf("Waiting for signal SIGINT or SIGQUIT...\n");
    /*发出相应的信号并跳转到信号处理函数处*/
    signal(SIGINT, SIG_IGN);  /*按键盘上的Ctrl+c组合键*/ // try SIG_DFL
    signal(SIGQUIT, my_func); /*按键盘上的Ctrl+\组合键*/
    pause();/*将调用进程挂起，直到捕捉到信号为止*/
    exit(0);
}


