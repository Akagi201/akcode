/**@file  client_check_file.h
 * @note  HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief 客户端中的检测升级文件的各个函数定义的头文件
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

#ifndef _CLIENT_CHECK_FILE_H
#define _CLIENT_CHECK_FILE_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>      // for mkdir
#include "upgrade_file.h"

#define _DEBUG

#ifdef  _DEBUG
	#define DBG_PRINT(x)       printf(x);                          
#else   
    #define DBG_PINT_EXIT(x)   ; 
#endif 

#define  BUFF_SIZE      1024           ///< 缓冲区
#define  FILE_NAME_LEN  128            ///< 文件名最大长度

int check_file(int sock, char *file_name);
int convert_data(char *p_src, char *p_dst, int len);
int check_file_checksum(char *memStart, unsigned int len, unsigned int checkSum);
int check_write_file(char *file_buffer, UPGRADE_FILE_HEADER *file_head, char *dir);



#endif
