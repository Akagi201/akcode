      
/** @file dining_philosophers_solve.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    哲学家就餐问题
  * 
  * @author   liuboyf1
  * @date     2012-9-17
  * @version  V1.0.0
  * 
  * @note ///Description here 
  * @note History:    
  * @note     <author>   <time>    <version >   <desc>
  * @note  
  * @warning  
  */

#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <pthread.h> 
#include <unistd.h> 
#include <semaphore.h> 

#define PHILOSOPHER_NUM 5
#define THINKING 1
#define HUNGRY 2
#define DINING 3


sem_t semph[PHILOSOPHER_NUM];   // 每个叉子有一个信号量
pthread_mutex_t mutex;          // Mutex for printing 


/** @brief    描述哲学家行为函数
  * @param[in]  代表哲学家的id
  * @param[out]  
  * @return  
  */
void* philosopher(void* param); 

int main(int argc, char* argv[]) 
{ 
    int i; 
    srand(getpid()); 
    pthread_t PhilosopherThread[PHILOSOPHER_NUM]; //定义线程数组
    int phId[PHILOSOPHER_NUM]; 
    pthread_mutex_init(&mutex, NULL); 
    for (i=0; i<PHILOSOPHER_NUM; i++) 
    { 
        phId[i] = i; 
		//对每个叉子信号量进行初始化为当前进程的局部信号量
        sem_init(&semph[i], 0, 1);
		//以philosopher为原型创建PHILOSOPHER_NUM个线程
        pthread_create(&PhilosopherThread[i], NULL, philosopher, (void*)(&phId[i])); 
        usleep(5000); 
    } 
    sleep(30); 
    return 0; 
} 

/** @brief    描述哲学家行为函数
  * @param[in]  代表哲学家的id
  * @param[out]  
  * @return  
  */

void* philosopher(void* param) 
{ 
    int myid; 
    char stateStr[128]; 
    char mystate; 
    int ret; 
    unsigned int leftFork; 
    unsigned int rightFork; 
    myid = *((int*)(param)); 
	printf("philosopher %d begin ....\n", myid);
    usleep(1000000); //把进程挂起一段时间， 单位是微秒（百万分之一秒）；

    // 初始状态为THINKING
    mystate = THINKING; 
    leftFork = (myid) % PHILOSOPHER_NUM; 
    rightFork = (myid + 1) % PHILOSOPHER_NUM; 
    while (1) 
    { 
        switch(mystate) 
        { 
        case THINKING:
            // 改变状态
            mystate = HUNGRY; 
            strcpy(stateStr, "HUNGRY"); 
            break; 
        case HUNGRY: 
            strcpy(stateStr, "HUNGRY"); 
            // 先测试左边叉子
            ret = sem_trywait(&semph[leftFork]); 
            if (ret == 0) 
            {
                // 左叉子可用, 拿起来 ,然后测试右边叉子 ... 
                ret = sem_trywait(&semph[rightFork]); 
                if (ret == 0) 
                { 
                    // 右边叉子也可用 ! 改变我的状态 
                    mystate = DINING; 
                    strcpy(stateStr, "DINING"); 
                } 
                else 
                { 
                    // 右边叉子被别人使用, 放下左边叉子.
                    sem_post(&semph[leftFork]); 
                } 
            } 
            break; 
        case DINING: 
            // 放下左边和右边叉子
            sem_post(&semph[leftFork]); 
            sem_post(&semph[rightFork]); 
            // 改变我的状态
            mystate = THINKING; 
            strcpy(stateStr, "THINKING"); 
            break; 
        } 
        // 打印我的状态
        pthread_mutex_lock(&mutex); 
		printf("philosopher %d is : %s\n", myid, stateStr); 
        pthread_mutex_unlock(&mutex); 

        // 睡眠一个随机时间(1 - 5s)
        int sleepTime;  
        sleepTime = 1 + (int)(5.0*rand()/(RAND_MAX+1.0));  
        usleep(sleepTime*100000); 
    } 
} 
