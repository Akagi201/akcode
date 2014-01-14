/**@file  client_recv_file.h
 * @note  HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief 客户端中的升级文件接收的各个函数定义的头文件 
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

#ifndef _CLIENT_RECV_FILE_H
#define _CLIENT_RECV_FILE_H

#include <netinet/in.h>    // for sockaddr_in
#include <sys/types.h>     // for socket
#include <sys/socket.h>    // for socket
#include <stdio.h>         // for printf
#include <stdlib.h>        // for exit
#include <string.h>        // for bzero
#include <pthread.h>       // for pthread_create
#include <dirent.h>        // for opendir
#include <errno.h>	
#include <assert.h>
#include <unistd.h>


#define FILE_NAME_SIZE   128       ///< 文件名最大长度
#define TCP_BUF_SIZE     4096      ///< TCP缓存大小

int client_sock_init(char *p_ip_addr, int port, int *sock, struct sockaddr_in *s_addr);
int client_recv_file(int sock, char *file_name, char **file_buffer, unsigned int *finish_len);

#endif
