      
/**@file chrdev.h
 * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief  定义设备结构体以及IOCTL参数
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
#define MEM_SIZE      0x00400000    // 全局内存最大4M字节

#endif

#ifndef CHRDEV_MAJOR
#define CHRDEV_MAJOR 0              // 默认的主设备号
#endif

#define CHRDEV_NUMS 1               // 设备的个数

// 设备结构体 
typedef struct 
{
    UINT8 *mem;                      // 全局内存 
    INT32 mem_size;                  // 全局内存的缓存大小
    UINT8 *mem_end;                  // 全局内存的结尾指针
    UINT8 *p_read;
    UINT8 *p_write;
    wait_queue_head_t read_queue;
    wait_queue_head_t write_queue;     
    struct semaphore sem;
    size_t cur_size;                // 设备当前存放的数据总量
    struct cdev cdev;
}MyCharDevice, *PMyCharDevice;

// 使用 't'作为IOCTL幻数
#define CHRDEV_IOC_MAGIC  't'

#define CHRDEV_IOC_BUFCLEAR    _IO(CHRDEV_IOC_MAGIC, 0)        // 将设备buffer清零
#define CHRDEV_IOC_SIZE        _IOR(CHRDEV_IOC_MAGIC, 1, int)  // 查询当前的数据总量

#if defined (__cplusplus) || defined (c_plusplus)
}
#endif

#endif // end of CHARDEVICE_DRV_CHRDEV_H_
