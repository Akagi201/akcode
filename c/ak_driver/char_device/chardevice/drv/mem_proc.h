      
/**@file mem_proc.h
 * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief  proc接口的声明文件
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
#define BUF_SIZE 1024UL // 用于proc接口写函数缓存

/** @fn
  * @brief	    创建/proc下的chrdev_proc节点
  * @param[in]  
  * @param[out]
  * @return
  */
void chrdev_create_proc(void);

/** @fn
  * @brief	    删除/proc下的chrdev_proc节点
  * @param[in]  
  * @param[out]
  * @return
  */
void chrdev_remove_proc(void);

 /** @fn
  * @brief	    proc的写函数接口，将用户层输入的数据在内核层打印出来
  * @param[in]  file : 未使用 buffer : 用户层传入的数据缓存 count : buffer大小 data : 未使用
  * @param[out]
  * @return     成功写入的字符数 or 相应失败代码
  */
ssize_t chrdev_write_proc(struct file *file, const char __user *buffer,
			   unsigned long count, void *data);

 /** @fn
  * @brief	    proc的读函数接口，读取chrdev字符设备驱动中的当前字符数、读写指针
  * @param[in]  page : 用于写入数据的缓存区 start : 未使用 off : 偏移量 count : 未使用 eof : 返回读结束标志
  * @param[out]
  * @return    总共返回的字符数(多设备时累加) or 相应失败代码
  */
ssize_t chrdev_read_proc(char *page, char **start, off_t off,
                   int count, int *eof, void *data);
#endif // end of CHARDEVICE_DRV_MEM_PROC_H_ 
