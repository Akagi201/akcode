      
/**@file file_opr.h
 * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief  �ļ���д������ͷ����
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
  * @brief	    ������������ȡ�û����ļ�������
  * @param[in]  filename : Ҫ��ȡ���ļ���  read_max_len : �����ȡ�ĳ���
  * @param[out] data : ��ȡ��������
  * @return	    0 : �ɹ� else : ʧ��
  */
INT32 file_read(const char * filename, UINT32 read_max_len, char * data);


/** @fn
  * @brief	    ����������д���û����ļ�������
  * @param[in]  filename : Ҫ��ȡ���ļ��� data : ����д������ݵ�ָ�� data_len : ׼��д������ݳ���
  * @param[out]
  * @return	    0 : �ɹ� else : ʧ��
  */

INT32 file_write(char* filename, char* data, UINT32 data_len);


#endif // end of CHARDEVICE_DRV_FILE_OPR_H_
