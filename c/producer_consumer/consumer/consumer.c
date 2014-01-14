      
/** @file consumer.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    消费者程序
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


int main(int argc,char **argv)
{
	int num;
	int shmid_goods,shmid_index,semid;
	char* shmaddr=NULL;
	int* indexaddr=NULL;
	int is_noexist=0;

	num=10;
	//create a shared memory as goods buffer
	if((shmid_goods=createshm(".",'s',num))==-1)
	{
		if(errno==EEXIST)
		{
			if((shmid_goods=openshm(".",'s'))==-1)
			{
				exit(1);
			}
		}
		else
		{
			perror("create shared memory failed\n");
		        exit(1);
		}
	}
	//attach the shared memory to the current process
	if((shmaddr=shmat(shmid_goods,(char*)0,0))==(char*)-1)
	{
		perror("attach shared memory error\n");
		exit(1);
	}

	//create a shared memory as index
	if((shmid_index=createshm(".",'z',2))==-1)
	{
		if(errno==EEXIST)
		{
			if((shmid_index=openshm(".",'z'))==-1)
			{
				exit(1);
			}
		}
		else
		{
			perror("create shared memory failed\n");
		        exit(1);
		}
	}
	else
	{
		is_noexist=1;
	}
	//attach the shared memory to the current process
	if((indexaddr=shmat(shmid_index,(int*)0,0))==(int*)-1)
	{
		perror("attach shared memory error\n");
		exit(1);
	}
	if(is_noexist)
	{
		indexaddr[0]=0;
		indexaddr[1]=0;
	}

	//create a semaphore set including 3 semaphores
	if((semid=createsem(".",'t',3,0))==-1)
	{
		if(errno==EEXIST)
		{
			if((semid=opensem(".",'t'))==-1)
			{
				exit(1);
			}
		}
		else
		{
			perror("semget error:");
			exit(1);
		}
	}
	else
	{
		union semun arg;
	        //seting value for mutex semaphore
         	arg.val=1;
         	if(semctl(semid,0,SETVAL,arg)==-1)
         	{
			perror("setting semaphore value failed\n");
	        	return -1;
            	}
          	//set value for synchronous semaphore
	        arg.val=num;
            	//the num means that the producer can continue to produce num products
          	if(semctl(semid,1,SETVAL,arg)==-1)
            	{
			perror("setting semaphore value failed\n");
	         	return -1;
         	}
           	//the last semaphore's value is default
           	//the default value '0' means that the consumer is not use any product now
      	}

	int goods=0;
	while(1)
	{
		p(semid,2);
		sleep(1);
		p(semid,0);
		//consumer is consuming a product
		goods=shmaddr[indexaddr[1]];
		printf("consumer:%d consumes a product[%d]:%d\n",getpid(),indexaddr[1],goods);
		indexaddr[1]=(indexaddr[1]+1)%num;
		v(semid,0);
		sleep(1);
		v(semid,1);
	}
	return 0;
}
