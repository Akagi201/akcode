/**@file  client_heart.h
 * @note  HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief �ͻ����е���������ĸ������������ͷ�ļ� 
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


int isRecv;                          ///< �ڱ�����Ƿ���������״̬��1-������0-��ʧ
static int cli_sock;                 ///< ȫ�ֱ������ͻ����׽���������
static int maxtime;                  ///< ȫ�ֱ��������Ӷ�ʱʱ��

static void sig_urg(int signo);
static void sig_alrm(int signo);
void heartbeat_cli(int sock, int maxtime_arg);

#endif
