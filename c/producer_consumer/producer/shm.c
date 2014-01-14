      
/** @file shm.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    
  * 
  * @author   liuboyf1
  * @date     2012-9-22
  * @version  V1.0.0
  * 
  * @note ///Description here 
  * @note History:    
  * @note     <author>   <time>    <version >   <desc>
  * @note  
  * @warning  
  */

#include "shm.h"
  
//create a semaphore set
int createsem(const char *pathname,int proj_id,int num,int init_val)
{
    key_t key;
    int i,semid;
    union semun arg;

    if((key=ftok(pathname,proj_id))==-1)
    {
        perror("ftok error:");
        return -1;
    }

    if((semid=semget(key,num,IPC_CREAT|IPC_EXCL|0666))==-1)
    {
        return -1;
    }

    //initialize the value of semaphore
    arg.val=init_val;
    for(i=0;i<num;i++)
    {
        if(semctl(semid,i,SETVAL,arg)==-1)
        {
            perror("semctl error:");
            return -1;
        }
    }

    return (semid);
}

//open the semaphore set
int opensem(const char*pathname,int proj_id)
{
    key_t key;
    int semid;

    if((key=ftok(pathname,proj_id))==-1)
    {
        perror("ftok error:");
        return -1;
    }

    //just get the id of semaphore set
    if((semid=semget(key,0,IPC_CREAT|0666))==-1)
    {
        perror("semget error:");
        return -1;
    }

    return (semid);
}

//P operation
int p(int semid,int index)
{
    struct sembuf buf={0,-1,0};

    if(index<0)
    {
        printf("error:the index is invalid\n");
        return -1;
    }

    buf.sem_num=index;
    if(semop(semid,&buf,1)==-1)
    {
        perror("semop error:");
        return -1;
    }

    return 1;

}

//V opeation
int v(int semid,int index)
{
    struct sembuf buf={0,+1,0};

    if(index<0)
    {
        printf("error:the index is invalid\n");
        return -1;
    }

    buf.sem_num=index;
    if(semop(semid,&buf,1)==-1)
    {
        perror("semop error:");
        return -1;
    }

    return 1;
}

//delete the semaphore set
int deletesem(int semid)
{
    return (semctl(semid,0,IPC_RMID,0)==-1);
}

//waiting for the semaphore is equal to 1
int waitsem(int semid,int index)
{
    while(semctl(semid,index,GETVAL,0)==0)
    {
        sleep(1);
        printf("I am waiting for semval equals 1..\n");
    }
    return 1;
}

//create share memory
int createshm(char *pathname,int proj_id,size_t size)
{
    key_t key;
    int shmid;

    if((key=ftok(pathname,proj_id))==-1)
    {
        perror("ftok error:");
        return -1;
    }

    if((shmid=shmget(key,size,IPC_CREAT|IPC_EXCL|0666))==-1)
    {
        return -1;
    }

    return (shmid);
}
//open share memory
int openshm(char *pathname,int proj_id)
{
    key_t key;
    int shmid;

    if((key=ftok(pathname,proj_id))==-1)
    {
        perror("ftok error:");
        return -1;
    }

    if((shmid=shmget(key,0,IPC_CREAT|0666))==-1)
    {
        perror("shmget error:");
        return -1;
    }

    return (shmid);
}

