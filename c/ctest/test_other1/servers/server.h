/**	@file server.h 
 *	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief 服务器API函数 
 *
 *	@author		zhoualei
 *	@date		2012/09/08
 *
 *	@note 服务器管理内存及创建的API函数 
 *	@note 多进度条打印实现多写一读的互斥读写数据块 
 */
#ifndef _SERVER_H_
#define _SERVER_H_

#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <pthread.h>
#include"map.h"
#include"ringbuffer.h"

#define BACKLOG 32
#define READQUEUE 32
#define READBLOCKSIZE 60
#define RINGBUFFER_DEFAULT 	1024	

#define SOCKET_INVALID 0	/*可用*/
#define SOCKET_CLOSED 1		/*关闭*/ 
#define SOCKET_SUSPEND 2	/*挂起*/   
#define SOCKET_READ 3		/*读取*/ 
#define SOCKET_POLLIN 4		/*写入*/ 

pthread_rwlock_t sock_mutex;
pthread_mutex_t	prin_mutex;

typedef struct 
{
	int fd;				/*sockfd*/ 
	RBUF_BLOCK * node;	/*存放一个链表，是不同时期收到的数据流*/
	RBUF_BLOCK * temp;	/*如果外部请求的数据块不连续，就重新申请一块连续空间temp，复习过去返回*/ 
	int status;			/*sockfd的状态*/		
	int pthread_flag;	/*处理此sockfd的线程状态*/
	int recev_filesize;/*表示之前此sockfd已经接收了多少的数据*/
	int send_ready_filesize;/*已经发送的文件的大小*/
	int filesize_total;/*需要发送的文件总大小*/ 
} SOCKET;

typedef struct 
{
	int listen_fd;	
	int epoll_fd;
	int max_connection;
	int closed;				/*当前客户端是否需要关闭*/ 
	int active;				/*当前需要处理的连接编号*/ 
	int skip;				/*数据在数据块中的偏移*/
 	volatile int pthread_print;/*当前有多少客户端线程在运行*/ 
	SOCKET * sockets;		/*指向在使用sock结构的指针*/ 
	SOCKET * free_socket;	/*指向可以使用的sock结构的指针*/ 
	HASH_MAP * socket_hash;	/*指向hash表用来寻找sock结构*/ 
	int queue_len;			/*一次轮训中产生的事件队列总数*/ 
	int queue_head;			/*队列的头*/ 
	struct epoll_event ev[READQUEUE];
	RINGBUFFER * rb;		/*环形缓冲区指针*/ 
} MREAD_POLL;
// create a pool , listen on port , set max connection and , buffer size (0 for default 1K bytes)
MREAD_POLL * mread_create(int port , int max , int buffer);
void mread_close(MREAD_POLL *self);

int mread_poll(MREAD_POLL *self , int timeout);
void * mread_pull(MREAD_POLL *self , int size);
void mread_yield(MREAD_POLL *self);
int mread_closed(MREAD_POLL *self);
int mread_socket(MREAD_POLL *self , int index);
void _close_active(MREAD_POLL * self);
void _close_client(MREAD_POLL * self, int id);
#endif
