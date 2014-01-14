      
/** @file mq_send.c
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
  
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    mqd_t mqd;
    char *ptr;
    size_t len;
    unsigned int prio;

    if(argc != 4)
    {
        perror("./mq_send mqueue_name size prio!\n");
    }
    len = atoi(argv[2]);
    prio = atoi(argv[3]);

    mqd = mq_open(argv[1], O_WRONLY);

    ptr = (char *)calloc(len, sizeof(char));
    strcpy(ptr, "Akagi201");
    mq_send(mqd, ptr, len, prio);

    exit(0);
}


