      
/** @file dining_philosophers_solve1.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    
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

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

#define PhNum    5
#define LEFT    (PhNum+argi-1)%PhNum
#define RIGHT    (argi+1)%PhNum
#define THINKING  0
#define HUNGRY    1
#define EATING    2
#define THINKTIME    4
#define EATTIME   3

union semun
{
    int val;
    struct semid_ds *buf;
    ushort *array;
};

void p_operate(int semid,ushort semnum);
void v_operate(int semid,ushort semnum);
void think(int argi);
void eat(int argi);
void test(int argi,char *state,int semid);
void take_forks(int argi,char *state,int semid);
void put_forks(int argi,char *state,int semid);
void philosopher(int argi,char *state,int semid);

int main(int argc,char *argv[])
{
    int semkey,shmkey,semid,shmid,fori,forki,mynum;
    char *state; 
    union semun arg1; 
    struct shmid_ds arg2;
    ushort array[PhNum+1];
    if((semid=semget(IPC_PRIVATE,PhNum+1,0600|IPC_CREAT))==-1) 
    {
        perror("Semget error!");
        exit(1);
    }
    if((shmid=shmget(IPC_PRIVATE,PhNum,0600|IPC_CREAT))==-1)
    {
        semctl(semid,0,IPC_RMID,arg1);
        perror("Shmget error!");
        exit(1);
    }
    array[PhNum]=1;
    for(fori=0;fori<PhNum;fori++) array[fori]=0;
    /*设置信号量数组初值*/
    arg1.array=array;
    if ((semctl(semid,PhNum+1,SETALL,arg1))==-1) 
        /*设置各信号量的初始值*/
    { 
        semctl(semid,0,IPC_RMID,arg1);
        shmctl(shmid,IPC_RMID,&arg2);
        perror("Set default error!");
        exit(1);
    }
    if ((state=shmat(shmid,0,0))==(char *)-1){ 
        /*将共享内存映射到当前进程地址空间*/
        semctl(semid,0,IPC_RMID,arg1);
        shmctl(shmid,IPC_RMID,&arg2);
        perror("Shmat error!");
        exit(1);
    }
    for (fori=0;fori<PhNum;fori++) 
        state[fori]=THINKING; /*初始化状态数组*/
    signal(SIGINT,SIG_IGN); 
    /*将进程对中断信号的处理设为忽略*/
    for (fori=1;fori<=PhNum;fori++)
    { /*创建5个哲学家子进程*/
        while((forki=fork())==-1);
        if (!forki){ /*子进程执行代码*/
            mynum=fori-1;
            signal(SIGINT,SIG_DFL); 
            /*设置子进程对SIGINT信号的处理过程*/ 
            break;
        }
    }
    if(forki){
        while(wait((int *)0)!=-1); 
        /*等待子进程执行结束,此处的(char *)0相当于NULL，即忽略返回状态*/
        semctl(semid,0,IPC_RMID,arg1);
        shmdt(state);
        shmctl(shmid,IPC_RMID,&arg2);
        printf("\nI'm monitor.All is over...\n");
    }
    else philosopher(mynum,state,semid); /*子进程调用*/
}
void p_operate(int semid,ushort semnum) 
{
    struct sembuf thesops;
    thesops.sem_num=semnum;
    thesops.sem_op=-1; 
    thesops.sem_flg=SEM_UNDO;
    //thesops.sem_flg=0; 
    semop(semid,&thesops,1); 
}
void v_operate(int semid,ushort semnum) 
{
    struct sembuf thesops;
    thesops.sem_num=semnum;
    thesops.sem_op=1; 
    thesops.sem_flg=SEM_UNDO;
    //  thesops.sem_flg=0; 
    semop(semid,&thesops,1); 
}

void think(int argi)
{
    printf("I'm philosopher #%d,I will think for %d second(s)!\n", 
            argi, argi + THINKTIME);
    sleep(argi + THINKTIME);
}
void eat(int argi)
{
    printf("I'm philosopher #%d,I will eat for %d seconds\n",argi,argi+EATTIME);
    sleep(argi+EATTIME);
}
void test(int argi,char *state,int semid)
{
    if(state[argi]==HUNGRY&&state[LEFT]!=EATING&&state[RIGHT]!=EATING)
    {
        state[argi]=EATING;
        printf("Hehe,philosopher #%d can eat now\n",argi);
        v_operate(semid,argi);
    }
    else 
        if (state[argi]!=HUNGRY)
            printf("Faint,philosopher #%d don't want to eat\n",argi);
        else printf("Oh,philosopher #%d can't eat now!\n",argi);
}
void take_forks(int argi,char *state,int semid)
{
    p_operate(semid,PhNum);
    state[argi]=HUNGRY;
    printf("Philosopher #%d is hungry.Can he eat...\n",argi);
    test(argi,state,semid);
    v_operate(semid,PhNum);
    p_operate(semid,argi);
}
void put_forks(int argi,char *state,int semid)
{
    p_operate(semid,PhNum);
    state[argi]=THINKING;
    printf("Philosopher #%d eat O.K..Can his neighbours eat...\n",argi);
    test(LEFT,state,semid);
    test(RIGHT,state,semid);
    v_operate(semid,PhNum);
}
void philosopher(int argi,char *state,int semid)
{
    while(1) 
    {
        think(argi);
        take_forks(argi,state,semid);
        eat(argi);
        put_forks(argi,state,semid);
    }
}
