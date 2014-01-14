/**@file  client_heart.c
 * @note  HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief 客户端中的心跳程序的各个函数定义 
 *
 * @author		yanghongwen
 * @date		2012/09/14
 * @version  V1.0
 *
 * @note ///Description here 
 * @note History:        
 * @note     <author>   <time>    <version >   <desc>
 * @note  
 * @warning 
 */

#include "client_heart.h"

/**@brief  闹钟信号处理函数，触发时表示服务器丢失
* @param signo
* @return  无
*/
static void sig_alrm(int signo)
{	
	isRecv = 0;
	return;		
}

/**@brief  TCP紧急信号处理函数，触发时表示接收到一个心跳，回应心跳后重置闹钟
* @param signo
* @return  无
*/
static void sig_urg(int signo)
{		
	char buf[12];	
	char c = 'R';
	int err = 0;
	
	bzero(buf, sizeof(buf));
	if(recv(cli_sock, buf, sizeof(buf), MSG_OOB) < 0)
	{
		printf("\nsig_urg :recv error\n");
		return ;
	}
	
	usleep(200000);
	err = send(cli_sock, &c, 1, MSG_OOB);			
	if(err < 0)
	{
		//客户端丢失，退出发送进程
		printf("\n\t\tsig_urg: send error\n");
		exit(0);
	}
	usleep(200000);		
	
	// 10S			
	alarm(maxtime);	
	return ;
}

/**@brief  心跳程序初始化
* @param sock 套接字描述符
* @param maxtime_arg 闹钟触发时间（单位：秒）
* @return  无
*/
void heartbeat_cli(int sock, int maxtime_arg)
{		
	cli_sock = sock;
	maxtime = maxtime_arg;
	
	//将套接字属主设为当前进程
	fcntl(sock, F_SETOWN, getpid());
	signal(SIGURG, sig_urg);
	signal(SIGALRM, sig_alrm);
	
	alarm(maxtime);
	return;
}

