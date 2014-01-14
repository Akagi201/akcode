      
/** @file producer.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    生产者模型
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


int main()
{
	int num;
	int shmid_goods,shmid_index,semid;
	char* shmaddr=NULL;
	int *indexaddr=NULL;
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
		p(semid,1);
		sleep(3);
		p(semid,0);
		//producer is producing a product
		goods=rand()%10;
		shmaddr[indexaddr[0]]=goods;
		printf("producer:%d produces a product[%d]:%d\n",getpid(),indexaddr[0],goods);
		indexaddr[0]=(indexaddr[0]+1)%10;
		v(semid,0);
		sleep(3);
		v(semid,2);
	}
	return  0;
}
