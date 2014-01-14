#include <pthread.h>
#include <semaphore.h> /* 使用posix信号量 */
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int number; // 被保护的全局变量
sem_t sem_id;

void* thread_one_fun(void *arg)
{
    sem_wait(&sem_id);
    printf("thread_one have the semaphore\n");
    number++;
    printf("number = %d\n",number);
    sem_post(&sem_id);
}
void* thread_two_fun(void *arg)
{
    sem_wait(&sem_id);
    printf("thread_two have the semaphore \n");
    number--;
    printf("number = %d\n",number);
    sem_post(&sem_id);
}
int main(int argc,char *argv[])
{
	int ret;
    number = 1;
    pthread_t id1, id2;
    ret = sem_init(&sem_id, 0, 1); /* 初始化信号量 */
	if (ret != 0)
    {
        perror("Semaphore initialization failed");
    }
    printf("sem_init\n");
    ret = pthread_create(&id1,NULL,thread_one_fun, NULL);
    if (ret != 0)
    {
        perror("Thread1 creation failure");
    }
    printf("Thread1 created\n");
    printf("sleep 10ms\n");
    usleep(10000);
    ret = pthread_create(&id2,NULL,thread_two_fun, NULL);
    if (ret != 0)
    {
        perror("Thread2 creation failure");
    }
    printf("Thread2 created\n");
    
    pthread_join(id1,NULL);
    pthread_join(id2,NULL);
    printf("main...\n");
    return 0;
}

