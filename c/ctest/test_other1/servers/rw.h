/**	@file rw.h
 *	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief ���ݶ�д��װ 
 *
 *	@author		zhoualei
 *	@date		2012/08/30
 *
 *	@note ���ļ���д���ݰ��ᶪʧ��װһ���д���ⶪ�� 
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
	//char filename[10]/*�ļ���*/	 
	int  filesize;	/*�ļ��ܴ�С*/
	//int  skip;		/*��Ҫ���͵Ĵ�С*/
	
}FILESIZE;

typedef struct
{
	int type;	/*0:��ʾ������1����ʾ�ļ�����*/
	int data_size;/*���η��͵����ݳ���*/ 
}CMD_FILE;
typedef struct
{
	int	recev_filesize;
}REQUEST_FROM_CLIENT;
int readall(int fd, void* buf, int *len);
int writeall(int fd, void* buf, int *len);
int send_file(SOCKET *sock, char *file_name);

#endif
