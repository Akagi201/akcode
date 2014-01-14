/**	@file rw.h
 *	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief 数据读写封装 
 *
 *	@author		zhoualei
 *	@date		2012/08/30
 *
 *	@note 大文件读写数据包会丢失封装一层读写避免丢包 
 *	@note 
 */
#ifndef _RW_H_
#define _RW_H_
#include"server.h"
#define	FILE_NAME_LEN	100
#define MAX_LEN			10*1024
#define MAX_UPGRADE_FILE	1024*1024*256
#define MAXSLEEP			128
typedef struct
{	
	//char filename[10]/*文件名*/	 
	int  filesize;	/*文件总大小*/
	//int  skip;		/*需要发送的大小*/
	
}FILESIZE;

typedef struct
{
	int type;	/*0:表示心跳；1：表示文件数据*/
	int data_size;/*本次发送的数据长度*/ 
}CMD_FILE;
typedef struct
{
	int	recev_filesize;
}REQUEST_FROM_CLIENT;
int readall(int fd, void* buf, int *len);
int writeall(int fd, void* buf, int *len);
int send_file(SOCKET *sock, char *file_name);

#endif
