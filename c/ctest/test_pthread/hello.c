
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

void *thread(void *vargp);

int main(int argc, char *argv[])
{
    pthread_t tid;
    //sleep(2);
    pthread_create(&tid, NULL, thread, NULL);
    printf("main:thread tid= %lu\n",tid);
    printf("main thread before pthread_join!\n");
    pthread_join(tid, NULL);  // 这样子线程执行完之后，主线程才执行
    printf("Main thread after pthread_join!\n");
    exit(0);
}

void *thread(void *vargp)
{
    sleep(3);
    printf("Hello world!\n");
    printf("thread tid = %lu\n", pthread_self());
    return NULL;
}
