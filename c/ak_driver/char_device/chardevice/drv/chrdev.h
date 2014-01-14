      
/**@file chrdev.h
 * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief  �����豸�ṹ���Լ�IOCTL����
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


#ifndef CHARDEVICE_DRV_CHRDEV_H_
#define CHARDEVICE_DRV_CHRDEV_H_


#if defined (__cplusplus) || defined (c_plusplus)
extern "C" {
#endif

#include <linux/ioctl.h>
#include <linux/cdev.h> 
#include "global.h"

#ifndef MEM_SIZE
#define MEM_SIZE      0x00400000    // ȫ���ڴ����4M�ֽ�

#endif

#ifndef CHRDEV_MAJOR
#define CHRDEV_MAJOR 0              // Ĭ�ϵ����豸��
#endif

#define CHRDEV_NUMS 1               // �豸�ĸ���

// �豸�ṹ�� 
typedef struct 
{
    UINT8 *mem;                      // ȫ���ڴ� 
    INT32 mem_size;                  // ȫ���ڴ�Ļ����С
    UINT8 *mem_end;                  // ȫ���ڴ�Ľ�βָ��
    UINT8 *p_read;
    UINT8 *p_write;
    wait_queue_head_t read_queue;
    wait_queue_head_t write_queue;     
    struct semaphore sem;
    size_t cur_size;                // �豸��ǰ��ŵ���������
    struct cdev cdev;
}MyCharDevice, *PMyCharDevice;

// ʹ�� 't'��ΪIOCTL����
#define CHRDEV_IOC_MAGIC  't'

#define CHRDEV_IOC_BUFCLEAR    _IO(CHRDEV_IOC_MAGIC, 0)        // ���豸buffer����
#define CHRDEV_IOC_SIZE        _IOR(CHRDEV_IOC_MAGIC, 1, int)  // ��ѯ��ǰ����������

#if defined (__cplusplus) || defined (c_plusplus)
}
#endif

#endif // end of CHARDEVICE_DRV_CHRDEV_H_
