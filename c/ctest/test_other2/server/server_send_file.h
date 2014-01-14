/**@file  server_send_file.h
 * @note  HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief 服务器中的发送升级包的各个函数定义的头文件 
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

#define FILE_DIR_NUM         30        //目录里文件数
#define FILE_NAME_SIZE       128       //文件名最大长度
#define TCP_LISTEN_QUEUE     20        //TCP监听请求数
#define TCP_BUF_SIZE         4096      //TCP缓存大小

void sig_sigpipe(int signo);
int server_sock_init(int port, int *p_sock);
int server_send_file(int sock, char *cli_ip, int cli_port);


#endif
