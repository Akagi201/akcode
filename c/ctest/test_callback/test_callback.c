
/** @file test_callback.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    回调函数，或简称回调，是指通过函数参数传递到其它代码的，某一块可执行代码的引用。这一设计允许了底层代码调用在高层定义的子程序。
  *
  * @author   liuboyf1
  * @date     2012-11-1
  * @version  V1.0.0
  *
  * @note ///Description here
  * @note History:
  * @note     <author>   <time>    <version >   <desc>
  * @note
  * @warning
  */

/* @brief 回调通常与原始调用者处于相同的抽象层
 */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void sig(int signum)
{
    printf("Received signal number %d!\n", signum);
}

int main(int argc, char *argv[])
{
    signal(SIGUSR1, sig);
    pause();
    return 0;
}

