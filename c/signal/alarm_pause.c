/*
 * @file alarm_pause.c
 * @author Akagi201
 * @date 2014/10/29
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    /* 调用alarm()定时器函数 */
    int ret = -1;
    ret = alarm(5);

    //pause();
    printf("I have been awaken up\n"); // 此语句不会被执行

    return 0;
}
