/**
 * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief 升级头文件
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
 
#define MAX_FILE_NUMS	32 ///< 升级包中最大文件数
 
 
/**@brief 整个升级包的报文头
 */
typedef struct
{
	unsigned int magic_number;	///< 魔术字，0x484b5753
	unsigned int header_checksum;	///< 报文头校验和，从file_nums字段开始计算
	unsigned int header_length;	///< 报文头总长度
	unsigned int file_nums;		///< 包含的文件个数
	unsigned int language;		///< 升级包语言，1-英文，2-中文
	unsigned int device_class;	///< 1 – DS9000 DVR, 2 - IPC
	unsigned int oem_code;		///< 1 – hikvision
	char         res[36];		///< 保留字段
}FIRMWARE_HEADER;
 
/**@brief 每个升级文件的报文头
 */
typedef struct
{
	char file_name[32];	///< 文件名
	int  start_offset;	///< 文件起始位置(在升级包中的偏移量)
	int  file_len;		///< 文件长度
	int  checksum;		///< 这个升级文件的校验和，不包含该报文头
}UPGRADE_FILE_HEADER;
 
#endif // _UPGRADE_FILE_H
