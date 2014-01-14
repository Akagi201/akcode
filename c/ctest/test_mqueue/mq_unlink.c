      
/** @file mq_unlink.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    
  * 
  * @author   liuboyf1
  * @date     2012-10-12
  * @version  V1.0.0
  * 
  * @note ///Description here 
  * @note History:    
  * @note     <author>   <time>    <version >   <desc>
  * @note  
  * @warning  
  */

#include<stdio.h>
#include<mqueue.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdlib.h>

int main(int argc, char **argv)
{
    int flag;
    if(argc != 2)
    {
        perror("usage: mq_unlink <name>");
    }
    flag = mq_unlink(argv[1]);
    if(flag == -1)
    {
        printf("fail!\n");
    }
    else
    {
        printf("ok!\n");
    }
    exit(0);
}

