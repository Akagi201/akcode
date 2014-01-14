/**@file upgrade_file.h
 * @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @brief 升级头文件
 * 
 * @author   cairuiqing
 * @date     2012-8-28
 * @version  1.0
 * 
 * @note ///Description here 
 * @note History:        
 * @note     <author>   <time>    <version >   <desc>
 * @note  
 * @warning  
 */

#ifndef _UPGRADE_FILE_H
#define _UPGRADE_FILE_H

#define  BUFFSIZE        1024
#define  TRUE            1
#define  FALSE           0
#define  ENDBUFFSIZE     32 ///< 发送升级进度用的缓冲区数据大小
#define  MAX_FILE_NUMS	 32 ///< 升级包中最大文件数

typedef  unsigned long int  INT32;      /* Unsigned 32 bit value */ 
typedef  unsigned char      UINT8;       /* Unsigned 8  bit value */ 

/**@brief 整个升级包的报文头
 */
typedef struct
{
	unsigned int magic_number;		///< 魔术字，0x484b5753
	unsigned int header_checksum;	///< 报文头校验和
	unsigned int header_length;		///< 报文头长度
	unsigned int file_nums;			///< 包含的文件个数
	unsigned int language;			///< 升级包语言，1-英文，2-中文
	unsigned int device_class;		///< 1 –DS9000 DVR, 2 - IPC
  unsigned int oem_code;			///< 1 –hikvision
	unsigned char         res[36];			///< 保留字段
}FIRMWARE_HEADER;

/**@brief 每个升级文件的报文头
 */
typedef struct
{
	char file_name[32];	///< 文件名
	int  start_offset;	///< 文件起始位置(在升级包中的偏移量)
	int  file_len;		///< 文件长度
	int  checksum;		///< 升级文件的校验和，不包含该报文头
}UPGRADE_FILE_HEADER;
int   tcpclien_heartbeat_fun(int fd , int sec , int maxsec);
int   tcpclien_recvdata(int fd ,char *filep,int filesize);
char* tcpclien_finddatahead(char *filep,int filesize);
int   tcpclien_decryptionfile(char *filep , int filenum);
int   tcpclien_getfile(char *filep , int filenum,int fd,int *index);
int   tcpclien_getfilenums(char *filep);
#endif // _UPGRADE_FILE_H


