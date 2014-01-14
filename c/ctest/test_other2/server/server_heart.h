/**@file  server_heart.h
 * @note  HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief �������е���������ĸ������������ͷ�ļ� 
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

#ifndef _SERVER_HEART_H
#define _SERVER_HEART_H


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <strings.h>
#include <time.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

static int ser_sock;          ///< �������׽���
static int alarm_time;        ///< ����ʱ�������������
static int max_times;         ///< ���������ʧ��
static int heart_count;       ///< ������ʧ������

void sig_chld(int signo);
static void sig_alrm(int signo);
static void sig_urg(int signo);
void heartbeat_serv(int sock_arg, int alarm_time_arg, int max_times_arg);


#endif
