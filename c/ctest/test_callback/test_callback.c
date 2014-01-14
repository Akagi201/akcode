
/** @file test_callback.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    �ص����������ƻص�����ָͨ�������������ݵ���������ģ�ĳһ���ִ�д�������á���һ��������˵ײ��������ڸ߲㶨����ӳ���
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

/* @brief �ص�ͨ����ԭʼ�����ߴ�����ͬ�ĳ����
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

