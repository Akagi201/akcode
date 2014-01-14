/**@file upgrade_file.h
 * @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @brief ����ͷ�ļ�
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
#define  ENDBUFFSIZE     32 ///< �������������õĻ��������ݴ�С
#define  MAX_FILE_NUMS	 32 ///< ������������ļ���

typedef  unsigned long int  INT32;      /* Unsigned 32 bit value */ 
typedef  unsigned char      UINT8;       /* Unsigned 8  bit value */ 

/**@brief �����������ı���ͷ
 */
typedef struct
{
	unsigned int magic_number;		///< ħ���֣�0x484b5753
	unsigned int header_checksum;	///< ����ͷУ���
	unsigned int header_length;		///< ����ͷ����
	unsigned int file_nums;			///< �������ļ�����
	unsigned int language;			///< ���������ԣ�1-Ӣ�ģ�2-����
	unsigned int device_class;		///< 1 �CDS9000 DVR, 2 - IPC
  unsigned int oem_code;			///< 1 �Chikvision
	unsigned char         res[36];			///< �����ֶ�
}FIRMWARE_HEADER;

/**@brief ÿ�������ļ��ı���ͷ
 */
typedef struct
{
	char file_name[32];	///< �ļ���
	int  start_offset;	///< �ļ���ʼλ��(���������е�ƫ����)
	int  file_len;		///< �ļ�����
	int  checksum;		///< �����ļ���У��ͣ��������ñ���ͷ
}UPGRADE_FILE_HEADER;
int   tcpclien_heartbeat_fun(int fd , int sec , int maxsec);
int   tcpclien_recvdata(int fd ,char *filep,int filesize);
char* tcpclien_finddatahead(char *filep,int filesize);
int   tcpclien_decryptionfile(char *filep , int filenum);
int   tcpclien_getfile(char *filep , int filenum,int fd,int *index);
int   tcpclien_getfilenums(char *filep);
#endif // _UPGRADE_FILE_H


