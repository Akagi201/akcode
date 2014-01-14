/**
 * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief ����ͷ�ļ�
 * 
 * @author   cairuiqing
 * @date     2012-8-28
 * @version  1.0
 * 
 * @note History:        
 * @note     <author>   <time>    <version >   <desc>
 * @note  
 * @warning  
 */
 
#ifndef _UPGRADE_FILE_H
#define _UPGRADE_FILE_H
 
#define MAX_FILE_NUMS	32 ///< ������������ļ���
 
 
/**@brief �����������ı���ͷ
 */
typedef struct
{
	unsigned int magic_number;	///< ħ���֣�0x484b5753
	unsigned int header_checksum;	///< ����ͷУ��ͣ���file_nums�ֶο�ʼ����
	unsigned int header_length;	///< ����ͷ�ܳ���
	unsigned int file_nums;		///< �������ļ�����
	unsigned int language;		///< ���������ԣ�1-Ӣ�ģ�2-����
	unsigned int device_class;	///< 1 �C DS9000 DVR, 2 - IPC
	unsigned int oem_code;		///< 1 �C hikvision
	char         res[36];		///< �����ֶ�
}FIRMWARE_HEADER;
 
/**@brief ÿ�������ļ��ı���ͷ
 */
typedef struct
{
	char file_name[32];	///< �ļ���
	int  start_offset;	///< �ļ���ʼλ��(���������е�ƫ����)
	int  file_len;		///< �ļ�����
	int  checksum;		///< ��������ļ���У��ͣ��������ñ���ͷ
}UPGRADE_FILE_HEADER;
 
#endif // _UPGRADE_FILE_H
