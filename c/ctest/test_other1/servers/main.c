/**	@file main.c
 *	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief 服务器主流程 
 *
 *	@author		zhoualei
 *	@date		2012/09/04
 *
 *	@note 实现多客户端共享同一块ringbuffer。 
 *	@note 多进度条打印实现多写一读的互斥读写数据块 
 */
#include <stdlib.h> 
#include "server.h"
#include "rw.h"
#define ONECLIENT	0
#define TWOCLIENT	1
#define THRCLIENT	2
void drawper(int file_send, int file_total, char* spOut); 
void *deal_print_progress_bar(void *arg);
void *deal_request(void *arg);
static void servers(MREAD_POLL *m);
int my_itoa(int val, char* buf);

/**	@fn	int my_itoa(int val, char* buf)
 *	@brief	10进制int转字符串 
 *	@param val 数值.
 *	@param buf 字符串存放地址指针.
 *	@return	字符串长度 
 */
int my_itoa(int val, char* buf)
{
	const unsigned int radix = 10;// 10进制 
	char* p;
	unsigned int a; //every digit
	int len;
	char* b; //start of the digit char
	char temp;
	unsigned int u;
	p = buf;
	if (val < 0)
	{
		*p++ = '-';
		val = 0 - val;
	}
	u = (unsigned int)val;
	b = p;
	do
	{
		a = u % radix;
		u /= radix;
		*p++ = a + '0';
	} while (u > 0);
	len = (int)(p - buf);
	*p-- = 0;
	//swap
	do
	{
		temp = *p;
		*p = *b;
		*b = temp;
		--p;
		++b;
	} while (b < p);
	return len;
}


/**	@fn	void drawper(int file_recv, int file_total, char* spOut)
 *	@brief	打印客户端进度条的百分比信息 
 *	@param file_send 表示已经发送的数量 
 *	@param file_total 总数量 
 *	@param spOut 指向信息存放的地址.
 *	@return	N\A 
 */
void drawper(int file_send, int file_total, char* spOut)
{
	/*检查输入参数*/
	if(NULL == spOut)
	{
		return;
	} 
	static int iCrlmm = 0;
	int iPer = file_send * 100 / file_total;

	if(file_send == file_total)
	{
		sprintf(spOut, "[ %d%]\r",  iPer);
	}
	else if( iCrlmm == 0)
	{
 		sprintf(spOut, "[| %d%]\r",  iPer);
	}
	else if( iCrlmm == 1)
	{
		sprintf(spOut, "[/ %d%]\r",  iPer);
	}
	else if( iCrlmm == 2)
	{
		sprintf(spOut, "[- %d%]\r",  iPer);
	}
	else if( iCrlmm == 3)
	{
		sprintf(spOut, "[\\ %d%]\r", iPer);
	}
	if(iCrlmm == 3)
    {
		iCrlmm = 0;
    }
	else
    {
		iCrlmm ++;
    }
}
/**	@fn	void *deal_print_progress_bar(void *arg)
 *	@brief	打印客户端进度条信息的线程 
 *	@param arg 共享结构体变量MREAD_POLL 
 *	@return	N\A 
 */
void *deal_print_progress_bar(void *arg)
{
	/*检查参数*/
	if(NULL == arg)
	{
		pthread_exit(NULL);	
	} 
	MREAD_POLL *self = (MREAD_POLL *)arg;
	MREAD_POLL *myself = NULL;
	/*打印进度条相关*/ 
	int i = 0;
	char szStr[121];
	int iStrLen = 100;
	int size_temp = 0;
	int count_temp = 0;
	char szOut[20] = "";
	char count[10] = "";
	int flag_clear = 0;
	memset(count, '\0', sizeof(count));
	if(NULL == (myself = (MREAD_POLL *)malloc(sizeof(MREAD_POLL))))
	{
		perror("malloc");
	}
	for(;;)
	{
		/*读写锁，写锁用来互斥所有的读锁*/ 
		pthread_rwlock_wrlock(&sock_mutex);		
		while(self->pthread_print > 0)//self->pthread_print
		{
			flag_clear = 1;
			memcpy(myself, self, sizeof(MREAD_POLL));
			pthread_rwlock_unlock(&sock_mutex);
			/*隐藏光标*/ 
			printf("\033[?25l");
			for (i = 0;i < myself->max_connection;i++) 
			{
				memset(szStr, '=', 100);
				memset(szStr + 100, '\0', 21);
				if (myself->sockets[i].status == SOCKET_READ) 
				{
					count_temp = myself->sockets[i].fd;
					usleep(300);
					size_temp = 0;
					if(myself->sockets[i].filesize_total != 0)
					{
						size_temp = 100 * myself->sockets[i].send_ready_filesize / myself->sockets[i].filesize_total;
					}
					else
					{
						break;
					}
					memset( szOut, '\0', sizeof(szOut) );
					memset(szStr, '>', size_temp);
					drawper(size_temp, iStrLen, szOut);
					memcpy(szStr + iStrLen, szOut, strlen(szOut) );
					my_itoa(count_temp, count);
					/*互斥打印信息避免进度条打印乱掉*/ 
					pthread_mutex_lock(&prin_mutex);
					printf("\033[%sB",count);
					printf("sockfd:%d",myself->sockets[i].fd);
					fseek(stdout, 0 , SEEK_SET);
					fprintf(stdout, szStr);
					fflush(stdout);
					printf("\033[%sA",count);
					pthread_mutex_unlock(&prin_mutex);
				}
			}
			memset(count, '\0', sizeof(count));
			pthread_rwlock_wrlock(&sock_mutex);
		}
		pthread_rwlock_unlock(&sock_mutex);
		if((0 == self->pthread_print) && (1 == flag_clear))
		{
			/*当没有客户端时清屏*/ 
			printf("\033[2J");	
			flag_clear = 0;
		}
	}
	free(myself);
	myself =NULL;
	pthread_exit(NULL);	
}
/**	@fn	void *deal_request(void *arg)
 *	@brief	处理客户端请求，实现文件传输和心跳 
 *	@param arg 共享结构体SOCKET 
 *	@return	N\A 
 */
void *deal_request(void *arg)
{
	SOCKET *sock = (SOCKET*)arg;
	char file_name[FILE_NAME_LEN] ="digicap.dav";
	pthread_detach(pthread_self());
	if(send_file(sock, file_name) == -1)
	{
		/*发送文件或心跳终端，则重传或续传文件*/ 
		pthread_mutex_lock(&prin_mutex);
		printf("\n\n\nSend file error,Retransmission or Resume broken transfer \n");
		printf("\033[4A");
		pthread_mutex_unlock(&prin_mutex); 
	}
	pthread_exit(NULL);
}

static void servers(MREAD_POLL *mread)
{
	/*获取当前需要处理的客户端信息*/ 
	int id = mread_poll(mread,0);
	if (id >= 0) 
	{
			pthread_t th;
			/*获取当前客户端传来的信息*/ 
			char * buffer = mread_pull(mread, sizeof(REQUEST_FROM_CLIENT));
			REQUEST_FROM_CLIENT request_file;
			if (buffer == NULL) 
			{
				mread_closed(mread);
			}
			else
			{
				memcpy(&request_file,buffer,sizeof(REQUEST_FROM_CLIENT));
				/*表示发来的是升级信息*/ 
				if(request_file.recev_filesize < MAX_UPGRADE_FILE)
				{
					/*置客户端升级线程标志*/ 
					mread->sockets[mread->active].pthread_flag = 1;
					/*获取当前客户端已经收取了多少数据判断是新客户端还是旧客户端续传*/ 
					mread->sockets[mread->active].recev_filesize = request_file.recev_filesize;
					/*创建线程处理*/ 
					if(pthread_create(&th, NULL, deal_request, &mread->sockets[mread->active]) != 0)
					{
						perror("pthread_create");
					}
					/*在ringbuffer中去除此信息*/
					mread_yield(mread);
				}
				else if(request_file.recev_filesize == MAX_UPGRADE_FILE)
				{
					/*表示客户端已经升级成功*/ 
					pthread_mutex_lock(&prin_mutex);
					printf("\n\nsockfd:%dupgrade suc\n",mread->sockets[mread->active].fd);
					printf("\033[3A");
					pthread_mutex_unlock(&prin_mutex);
					/*在ringbuffer中去除此信息*/
					mread_yield(mread);
				}
				else/*用于扩展*/ 
				{
					/*在ringbuffer中去除此信息*/	
					mread_yield(mread);
				}
				
			}
	}
}

int main(int argc, char *argv[])
{
	/*检查参数*/
	if(argc != 2)
	{
		printf("Usage: server <port>\n");
		return 0;
	}
	/*初始化读写锁用来管理1线程读多线程写（写的块地址不一样）*/ 
	if(pthread_rwlock_init(&sock_mutex, NULL) != 0)
	{
		perror("rwlock initialization failed");
		return 1;
	}
	/*初始化打印锁用来互斥打印用于实现进度条的美观性*/ 
	if(pthread_mutex_init(&prin_mutex, NULL) != 0)
	{
		perror("Mutex initialization failed");
		return 1;
	}
	/*创建poll，监听端口，设置最大连接数，初始化ringbuffer、map、SOCKET结构体数组*/ 
	MREAD_POLL * mread = mread_create(atoi(argv[1]) , 40, 0);
	if (mread == NULL)
	{
		perror("error:");
		return 1;
	}
	pthread_t th_print;
	/*创建线程处理服务器进度条信息*/
	if(pthread_create(&th_print, NULL, deal_print_progress_bar, mread) != 0)
	{
		perror("pthread_create");
	}
	for (;;)
	{
		servers(mread);
		usleep(300);
	}
	pthread_rwlock_destroy(&sock_mutex);
	pthread_mutex_destroy(&prin_mutex);
	return 0;
}
