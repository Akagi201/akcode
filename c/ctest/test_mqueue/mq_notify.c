      
/** @file mq_notify.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    
  * 
  * @author   liuboyf1
  * @date     2012-10-12
  * @version  V1.0.0
  * 
  * @note ///Description here 
  * @note History:    
  * @note     <author>   <time>    <version >   <desc>
  * @note  
  * @warning  
  */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <signal.h>

mqd_t mqd;
void *buff;
struct mq_attr attr;
struct sigevent sigev;

static void sig_usr1(int);

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        perror("argument error!\n");
    }
    mqd = mq_open(argv[1], O_RDONLY);
    mq_getattr(mqd, &attr);  //获取属性
    buff = malloc(attr.mq_msgsize);

    signal(SIGUSR1, sig_usr1);//接收到SIGUSR1信号后，执行sig_usr1函数
    sigev.sigev_notify = SIGEV_SIGNAL;
    sigev.sigev_signo = SIGUSR1;

    mq_notify(mqd, &sigev); //注册

    for(;;)
    {
        printf("ddddd\n");
        sleep(3);
        //	pause();
    }
    exit(0);
}

static void sig_usr1(int signo)
{
    ssize_t n;

    mq_notify(mqd, &sigev);  //重新注册
    n = mq_receive(mqd, buff, attr.mq_msgsize, NULL);
    printf("SIGUSR1 received,read %ld bytes, buff %s\n", (long)n, (char *)buff);
    return;
}


