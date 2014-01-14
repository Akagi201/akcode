#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex; /* 创建互斥锁mutex */
int share = 0; /* 共享数据 */

/* 线程入口 */

void *thread_function(void *arg)
{
    int i;
    const int N = 1000000;
    for (i = 0; i < N; ++i)
    {
        /* 访问共享数据 */
        pthread_mutex_lock(&mutex);
        ++share;
        pthread_mutex_unlock(&mutex);
    }
}

int main(void)
{
    int i;
    pthread_mutex_init(&mutex, NULL); /* 初始化互斥锁mutex */
    pthread_t thread_id;
    void *exit_status;
    /* 创建新线程 */
    pthread_create(&thread_id, NULL, thread_function, NULL);
    for (i = 0; i < 10; ++i)
    {
        usleep(10000); /* 延时10ms */
        /* 访问共享数据 */
        pthread_mutex_lock(&mutex);
        printf("share:%d\n", share);
        pthread_mutex_unlock(&mutex);
    }
    pthread_join(thread_id, &exit_status); /* 等待线程结束 */
    pthread_mutex_destroy(&mutex); /* 销毁互斥锁 */
    return 0;
}
