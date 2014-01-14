      
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


/* ftp 上传服务器认证信息结构体 */
typedef strcut
{
	UINT32	bValid;				/*是否启动ftp上传功能*/
	struct in_addr ipAddr;		/*ftp 服务器*/
	UINT32	portNo;				/*ftp端口*/
	UINT8	usrname[NAME_LEN];	/*用户名*/
	UINT8	passwd[PASSWD_LEN];	/*密码*/
	UINT32	dirLevel;			/*0 = 不使用目录结构,1 = 使用1级目录,2=使用2级目录*/
	UINT16 	topDirMode;			/* 0x1 = 使用设备名,0x2 = 使用设备号,0x3 = 使用设备ip地址,0xff=用户自定义*/
	UINT16 	subDirMode;			/* 0x1 = 使用通道名,0x2 = 使用通道号,0xff=用户自定义*/
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


