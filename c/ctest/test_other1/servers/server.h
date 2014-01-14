/**	@file server.h 
 *	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief ������API���� 
 *
 *	@author		zhoualei
 *	@date		2012/09/08
 *
 *	@note �����������ڴ漰������API���� 
 *	@note ���������ӡʵ�ֶ�дһ���Ļ����д���ݿ� 
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

#define SOCKET_INVALID 0	/*����*/
#define SOCKET_CLOSED 1		/*�ر�*/ 
#define SOCKET_SUSPEND 2	/*����*/   
#define SOCKET_READ 3		/*��ȡ*/ 
#define SOCKET_POLLIN 4		/*д��*/ 

pthread_rwlock_t sock_mutex;
pthread_mutex_t	prin_mutex;

typedef struct 
{
	int fd;				/*sockfd*/ 
	RBUF_BLOCK * node;	/*���һ�������ǲ�ͬʱ���յ���������*/
	RBUF_BLOCK * temp;	/*����ⲿ��������ݿ鲻����������������һ�������ռ�temp����ϰ��ȥ����*/ 
	int status;			/*sockfd��״̬*/		
	int pthread_flag;	/*�����sockfd���߳�״̬*/
	int recev_filesize;/*��ʾ֮ǰ��sockfd�Ѿ������˶��ٵ�����*/
	int send_ready_filesize;/*�Ѿ����͵��ļ��Ĵ�С*/
	int filesize_total;/*��Ҫ���͵��ļ��ܴ�С*/ 
} SOCKET;

typedef struct 
{
	int listen_fd;	
	int epoll_fd;
	int max_connection;
	int closed;				/*��ǰ�ͻ����Ƿ���Ҫ�ر�*/ 
	int active;				/*��ǰ��Ҫ��������ӱ��*/ 
	int skip;				/*���������ݿ��е�ƫ��*/
 	volatile int pthread_print;/*��ǰ�ж��ٿͻ����߳�������*/ 
	SOCKET * sockets;		/*ָ����ʹ��sock�ṹ��ָ��*/ 
	SOCKET * free_socket;	/*ָ�����ʹ�õ�sock�ṹ��ָ��*/ 
	HASH_MAP * socket_hash;	/*ָ��hash������Ѱ��sock�ṹ*/ 
	int queue_len;			/*һ����ѵ�в������¼���������*/ 
	int queue_head;			/*���е�ͷ*/ 
	struct epoll_event ev[READQUEUE];
	RINGBUFFER * rb;		/*���λ�����ָ��*/ 
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
