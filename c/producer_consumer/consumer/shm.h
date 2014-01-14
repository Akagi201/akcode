      
/** @file shm.h
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    多个生产者和多个消费者对N个缓冲区（N个货架）进行访问
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
 
#ifndef SHM_H_
#define SHM_H_


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>

#define SHM_SIZE 1024	

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int createsem(const char *pathname,int proj_id,int num,int init_val);
int opensem(const char*pathname,int proj_id);
int p(int semid,int index);
int v(int semid,int index);
int deletesem(int semid);
int waitsem(int semid,int index);
int createshm(char *pathname,int proj_id,size_t size);
int openshm(char *pathname,int proj_id);

#endif  // SHM_H_

