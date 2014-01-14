      
/** @file vx_ftp_client.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    ftp lib in vxworks
  * 
  * @author   liuboyf1
  * @date     2013-1-25
  * @version  V1.0.0
  * 
  * @note ///Description here 
  * @note History:    
  * @note     <author>   <time>    <version >   <desc>
  * @note  
  * @warning  
  */

#include <stdio.h>
#include "ftpLib.h"

#define MAX_SUPPORT_FTP (3)


/* ftp �ϴ���������֤��Ϣ�ṹ�� */
typedef strcut
{
	UINT32	bValid;				/*�Ƿ�����ftp�ϴ�����*/
	struct in_addr ipAddr;		/*ftp ������*/
	UINT32	portNo;				/*ftp�˿�*/
	UINT8	usrname[NAME_LEN];	/*�û���*/
	UINT8	passwd[PASSWD_LEN];	/*����*/
	UINT32	dirLevel;			/*0 = ��ʹ��Ŀ¼�ṹ,1 = ʹ��1��Ŀ¼,2=ʹ��2��Ŀ¼*/
	UINT16 	topDirMode;			/* 0x1 = ʹ���豸��,0x2 = ʹ���豸��,0x3 = ʹ���豸ip��ַ,0xff=�û��Զ���*/
	UINT16 	subDirMode;			/* 0x1 = ʹ��ͨ����,0x2 = ʹ��ͨ����,0xff=�û��Զ���*/
}FTP_UPLOAD_PARA;

FTP_UPLOAD_PARA ftp_all_upload =
{
	.bValid = TRUE;
	.portNo = 21;
	.usrname = "admin";
	.passwd = "12345";
};

int main(void)
{
	int ctrl_socket;
	int data_socket;
	char buf[512];
	int nbytes;
	
	if (ERROR ==ftpXfer())
	
	return 0;
}


