      
/** @file dining_philosophers_solve.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    ��ѧ�ҾͲ�����
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


sem_t semph[PHILOSOPHER_NUM];   // ÿ��������һ���ź���
pthread_mutex_t mutex;          // Mutex for printing 


/** @brief    ������ѧ����Ϊ����
  * @param[in]  ������ѧ�ҵ�id
  * @param[out]  
  * @return  
  */
void* philosopher(void* param); 

int main(int argc, char* argv[]) 
{ 
    int i; 
    srand(getpid()); 
    pthread_t PhilosopherThread[PHILOSOPHER_NUM]; //�����߳�����
    int phId[PHILOSOPHER_NUM]; 
    pthread_mutex_init(&mutex, NULL); 
    for (i=0; i<PHILOSOPHER_NUM; i++) 
    { 
        phId[i] = i; 
		//��ÿ�������ź������г�ʼ��Ϊ��ǰ���̵ľֲ��ź���
        sem_init(&semph[i], 0, 1);
		//��philosopherΪԭ�ʹ���PHILOSOPHER_NUM���߳�
        pthread_create(&PhilosopherThread[i], NULL, philosopher, (void*)(&phId[i])); 
        usleep(5000); 
    } 
    sleep(30); 
    return 0; 
} 

/** @brief    ������ѧ����Ϊ����
  * @param[in]  ������ѧ�ҵ�id
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
    usleep(1000000); //�ѽ��̹���һ��ʱ�䣬 ��λ��΢�루�����֮һ�룩��

    // ��ʼ״̬ΪTHINKING
    mystate = THINKING; 
    leftFork = (myid) % PHILOSOPHER_NUM; 
    rightFork = (myid + 1) % PHILOSOPHER_NUM; 
    while (1) 
    { 
        switch(mystate) 
        { 
        case THINKING:
            // �ı�״̬
            mystate = HUNGRY; 
            strcpy(stateStr, "HUNGRY"); 
            break; 
        case HUNGRY: 
            strcpy(stateStr, "HUNGRY"); 
            // �Ȳ�����߲���
            ret = sem_trywait(&semph[leftFork]); 
            if (ret == 0) 
            {
                // ����ӿ���, ������ ,Ȼ������ұ߲��� ... 
                ret = sem_trywait(&semph[rightFork]); 
                if (ret == 0) 
                { 
                    // �ұ߲���Ҳ���� ! �ı��ҵ�״̬ 
                    mystate = DINING; 
                    strcpy(stateStr, "DINING"); 
                } 
                else 
                { 
                    // �ұ߲��ӱ�����ʹ��, ������߲���.
                    sem_post(&semph[leftFork]); 
                } 
            } 
            break; 
        case DINING: 
            // ������ߺ��ұ߲���
            sem_post(&semph[leftFork]); 
            sem_post(&semph[rightFork]); 
            // �ı��ҵ�״̬
            mystate = THINKING; 
            strcpy(stateStr, "THINKING"); 
            break; 
        } 
        // ��ӡ�ҵ�״̬
        pthread_mutex_lock(&mutex); 
		printf("philosopher %d is : %s\n", myid, stateStr); 
        pthread_mutex_unlock(&mutex); 

        // ˯��һ�����ʱ��(1 - 5s)
        int sleepTime;  
        sleepTime = 1 + (int)(5.0*rand()/(RAND_MAX+1.0));  
        usleep(sleepTime*100000); 
    } 
} 
