      
/**@file file_opr.h
 * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief  文件读写函数的头定义
 * 
 * @author   liuboyf1
 * @date     2012-8-30
 * @version  V1.0.0
 * 
 * @note ///Description here 
 * @note History:        
 * @note     <author>   <time>    <version >   <desc>
 * @note  
 * @warning  
 */


#ifndef CHARDEVICE_DRV_FILE_OPR_H_
#define CHARDEVICE_DRV_FILE_OPR_H_
#include "global.h"

/** @fn
  * @brief	    驱动程序来读取用户层文件的内容
  * @param[in]  filename : 要读取的文件名  read_max_len : 请求读取的长度
  * @param[out] data : 读取到的数据
  * @return	    0 : 成功 else : 失败
  */
INT32 file_read(const char * filename, UINT32 read_max_len, char * data);


/** @fn
  * @brief	    驱动程序来写入用户层文件的内容
  * @param[in]  filename : 要读取的文件名 data : 请求写入的数据的指针 data_len : 准备写入的数据长度
  * @param[out]
  * @return	    0 : 成功 else : 失败
  */

INT32 file_write(char* filename, char* data, UINT32 data_len);


#endif // end of CHARDEVICE_DRV_FILE_OPR_H_
