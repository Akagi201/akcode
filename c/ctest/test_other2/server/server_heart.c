/**@file  server_heart.c
 * @note  HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief 服务器中的心跳程序的各个函数定义 
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

#include "server_heart.h"

/**@brief  闹钟信号处理函数，触发时向客户端发送一个心跳
* @param signo
* @return  无
*/
static void sig_alrm(int signo)
{    
    int i = 0;
	int err = 0;	 
	char c = 'A';
	
	usleep(200000);	
	err = send(ser_sock, &c, 1, MSG_OOB);			
	if(err < 0)
	{
		//客户端丢失，退出发送进程
		printf("\n\t\t心跳:客户端已关闭，退出发送进程\n");
		exit(0);
	}		
	usleep(200000);	
	
	if(heart_count++ > max_times)
	{
		printf("\n\t\t心跳:客户端已丢失，退出发送进程\n");
	}
	
	//重置闹钟
	alarm(alarm_time);
	return ;
}


/**@brief  TCP紧急信号处理函数，触发时表示接收到一个心跳回应，将心跳丢失计数器清零
* @param signo
* @return  无
*/
static void sig_urg(int signo)
{
	char c = 0;
	int err = 0;
	
	err = recv(ser_sock, &c, 1, MSG_OOB);
	if(err < 0)
	{
		printf("\n心跳接收: recv error\n");
	}
	
	heart_count = 0;
	fflush(stdout);	
}

/**@brief  SIGCHLD信号处理函数，回收子进程
* @param signo
* @return  无
*/
void sig_chld(int signo)
{
	pid_t pid = 0;
	int stat = 0;
	
	while((pid = waitpid(-1, &stat, WNOHANG)) > 0)
	{
		printf("\n\t\t发送子进程<%d> 已终止！\n", pid);		
	}
	return ;
}

/**@brief  心跳程序初始化
* @param sock 套接字描述符
* @param maxtime_arg 闹钟触发时间（单位：秒）
* @param maxtime_arg 最大心跳丢失数
* @return  无
*/
void heartbeat_serv(int sock_arg, int alarm_time_arg, int max_times_arg)
{
	ser_sock = sock_arg; 
	alarm_time = alarm_time_arg;
	max_times = max_times_arg;
	heart_count = 0;
	
	//将套接字属主设为当前进程
	fcntl(ser_sock, F_SETOWN, getpid());
	
	signal(SIGALRM, sig_alrm);	
	signal(SIGURG, sig_urg);
	alarm(alarm_time);
	return;
}


