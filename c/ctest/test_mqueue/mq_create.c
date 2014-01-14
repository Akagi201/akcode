      
/** @file mq_create.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    
  * 
  * @author   liuboyf1
  * @date     2012-10-11
  * @version  V1.0.0
  * 
  * @note ///Description here 
  * @note History:    
  * @note     <author>   <time>    <version >   <desc>
  * @note  
  * @warning  
  */
  
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<mqueue.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>

struct mq_attr attr;

int main(int argc , char **argv)
{
    int c;
    int flags;
    mqd_t mqd;

    flags = O_RDWR|O_CREAT;
    //emz表示命令行的选项，m和z带有参数
    while((c = getopt(argc, argv, "em:z:")) != -1)
    {
        switch(c)
        {

        case 'e':
            flags |= O_EXCL;
            break;
        case 'm':
            attr.mq_maxmsg = atol(optarg);
            break;
        case 'z':
            attr.mq_msgsize = atol(optarg);
            break;
        }
    }

    if(optind != argc - 1)
    {
        perror("optind error!\n");
    }

    if((attr.mq_maxmsg != 0 && attr.mq_msgsize == 0) || (attr.mq_maxmsg == 0 && attr.mq_msgsize != 0))
    {
        perror("must specify both -m maxmsg and -z msgsize!\n");
    }

    mqd = mq_open(argv[optind], flags, 0644, (attr.mq_maxmsg != 0) ? &attr : NULL);
    if(mqd != -1)
    {    
		printf("create ok!\n");
        mq_close(mqd);  //mq_close只是关闭了描述符，并不删除消息队列
    }
    else
    {
		mq_close(mqd);  //mq_close只是关闭了描述符，并不删除消息队列
        printf("create error!\n");
    }
    exit(0);

}

