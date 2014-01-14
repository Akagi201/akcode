      
/** @file two_sem.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    想要某个特定的顺序的话，可以用 2 个信号量来实现。例如下面的例程是线程 1 先执行完，然后线程 2 才继
续执行，直至结束
  * 
  * @author   liuboyf1
  * @date     2012-10-10
  * @version  V1.0.0
  * 
  * @note ///Description here 
  * @note History:    
  * @note     <author>   <time>    <version >   <desc>
  * @note  
  * @warning  
  */

#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int number;  /* 被保护的全局变量 */

sem_t sem_id1, sem_id2;
void* thread_one_fun(void *arg)
{
    sem_wait(&sem_id1);
    printf("thread_one have the semaphore\n");
    number++;
    printf("number = %d\n",number);
    sem_post(&sem_id2);
}
void* thread_two_fun(void *arg)
{
    sem_wait(&sem_id2);
    printf("thread_two have the semaphore \n");
    number--;
    printf("number = %d\n",number);
    sem_post(&sem_id1);
}
int main(int argc,char *argv[])
{
    number = 1;
    pthread_t id1, id2;
    sem_init(&sem_id1, 0, 1); // 空闲的
    sem_init(&sem_id2, 0, 0); // 忙的
    pthread_create(&id1,NULL,thread_one_fun, NULL);
    pthread_create(&id2,NULL,thread_two_fun, NULL);
    pthread_join(id1,NULL);
    pthread_join(id2,NULL);
    printf("main,,,\n");
    return 0;
}


