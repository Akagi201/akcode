/**@file  server_send_file.h
 * @note  HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief �������еķ����������ĸ������������ͷ�ļ� 
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

#ifndef _SERVER_SEND_FILE_H
#define _SERVER_SEND_FILE_H

#include <netinet/in.h>    // for sockaddr_in
#include <sys/types.h>     // for socket
#include <sys/socket.h>    // for socket
#include <arpa/inet.h>     // for inet_ntop
#include <stdio.h>         // for printf
#include <stdlib.h>        // for exit
#include <string.h>        // for bzero
#include <pthread.h>       // for pthread_create
#include <dirent.h>        // for opendir
#include <errno.h>	
#include <signal.h>
#include <assert.h>

#define FILE_DIR_NUM         30        //Ŀ¼���ļ���
#define FILE_NAME_SIZE       128       //�ļ�����󳤶�
#define TCP_LISTEN_QUEUE     20        //TCP����������
#define TCP_BUF_SIZE         4096      //TCP�����С

void sig_sigpipe(int signo);
int server_sock_init(int port, int *p_sock);
int server_send_file(int sock, char *cli_ip, int cli_port);


#endif
