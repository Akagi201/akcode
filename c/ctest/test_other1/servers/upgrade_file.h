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

#define MAX_FILE_NUMS	32 ///< ������������ļ���


#define PORT            	4321
#define BUFFER_SIZE         1024
#define MAX_QUE_CONN_NM     5
#define NAMELEN 			32


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
	char         res[36];			///< �����ֶ�
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

typedef unsigned char UINT8;
typedef unsigned int UINT32;
typedef int INT32;
#define FIRMWARE_HEADER_LEN	sizeof(FIRMWARE_HEADER)
#define FILE_HEADER_LEN		sizeof(UPGRADE_FILE_HEADER)

#endif // _UPGRADE_FILE_H


