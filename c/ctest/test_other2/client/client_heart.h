/**@file  client_heart.h
 * @note  HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief 客户端中的心跳程序的各个函数定义的头文件 
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

#ifndef _CLIENT_HEART_H
#define _CLIENT_HEART_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>

#include <netinet/in.h>
#include <arpa/inet.h>


int isRecv;                          ///< 哨兵，标记服务器连接状态，1-正常，0-丢失
static int cli_sock;                 ///< 全局变量，客户端套接字描述符
static int maxtime;                  ///< 全局变量，闹钟定时时长

static void sig_urg(int signo);
static void sig_alrm(int signo);
void heartbeat_cli(int sock, int maxtime_arg);

#endif
