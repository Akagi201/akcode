      
/**@file mem_proc.h
 * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief  proc�ӿڵ������ļ�
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

#ifndef CHARDEVICE_DRV_MEM_PROC_H_
#define CHARDEVICE_DRV_MEM_PROC_H_
#include "global.h"
#define BUF_SIZE 1024UL // ����proc�ӿ�д��������

/** @fn
  * @brief	    ����/proc�µ�chrdev_proc�ڵ�
  * @param[in]  
  * @param[out]
  * @return
  */
void chrdev_create_proc(void);

/** @fn
  * @brief	    ɾ��/proc�µ�chrdev_proc�ڵ�
  * @param[in]  
  * @param[out]
  * @return
  */
void chrdev_remove_proc(void);

 /** @fn
  * @brief	    proc��д�����ӿڣ����û���������������ں˲��ӡ����
  * @param[in]  file : δʹ�� buffer : �û��㴫������ݻ��� count : buffer��С data : δʹ��
  * @param[out]
  * @return     �ɹ�д����ַ��� or ��Ӧʧ�ܴ���
  */
ssize_t chrdev_write_proc(struct file *file, const char __user *buffer,
			   unsigned long count, void *data);

 /** @fn
  * @brief	    proc�Ķ������ӿڣ���ȡchrdev�ַ��豸�����еĵ�ǰ�ַ�������дָ��
  * @param[in]  page : ����д�����ݵĻ����� start : δʹ�� off : ƫ���� count : δʹ�� eof : ���ض�������־
  * @param[out]
  * @return    �ܹ����ص��ַ���(���豸ʱ�ۼ�) or ��Ӧʧ�ܴ���
  */
ssize_t chrdev_read_proc(char *page, char **start, off_t off,
                   int count, int *eof, void *data);
#endif // end of CHARDEVICE_DRV_MEM_PROC_H_ 
