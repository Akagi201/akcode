      
/**@file mem_proc.c
 * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief  proc接口的实现文件
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
 
#include <linux/proc_fs.h>
#include "mem_proc.h"
#include "chrdev.h"

extern PMyCharDevice g_chrdev_devices;   // 设备结构体指针，在chrdev_init_module() 中分配空间
extern INT32 g_chrdvr_nums;              // 字符设备个数

/** @fn
  * @brief	    proc的读函数接口，读取chrdev字符设备驱动中的当前字符数、读写指针
  * @param[in]  page : 用于写入数据的缓存区 start : 未使用 off : 偏移量 count : 未使用 eof : 返回读结束标志
  * @param[out]
  * @return	   总共返回的字符数(多设备时累加) or 相应失败代码
  */

ssize_t chrdev_read_proc(char *page, char **start, off_t off,
                   int count, int *eof, void *data)
{
    int ret = 0;
    int len = 0;
    int tmp_len = 0;
    int i = 0;

    if(NULL == page)
    {
        return -EFAULT;
    }

    if(off >0)// 不能偏移访问
    {
        *eof = 1;
        ret = 0;
    }
    else
    {
        for(i = 0; i<g_chrdvr_nums; i++)
        {
            // 格式化返回的字符
            tmp_len = sprintf(page + len, "\n devices[%d] cur_size: %d p_read :%0X p_write: %0X \n"
                              , i, g_chrdev_devices[i].cur_size, (UINT32)g_chrdev_devices[i].p_read
                              , (UINT32)g_chrdev_devices[i].p_write);
            DBG_PRINT("mem content is: \n %s \n", g_chrdev_devices[i].mem);
            len += tmp_len;
        }
        ret = len;
    }
    return ret;
}


/** @fn
 * @brief	   proc的写函数接口，将用户层输入的数据在内核层打印出来
 * @param[in]  file : 未使用 buffer : 用户层传入的数据缓存 count : buffer大小 data : 未使用
 * @param[out]
 * @return	   成功写入的字符数 or 相应失败代码
 */
ssize_t chrdev_write_proc(struct file *file, const char __user *buffer,
               unsigned long count, void *data)
{
    char k_buf[BUF_SIZE] = {0};
    INT32 ret = 0;
    INT32 len = min(BUF_SIZE, count);

    if(NULL == buffer)
    {
        return -EFAULT; 
    }
    // 用户空间--> 内核空间
    if(copy_from_user(k_buf, buffer, len ))
    {
        ret = -EFAULT;
    }
    else
    {
        // 输出用户从proc接口输入的数据
        DBG_PRINT("you have input: %s\n", k_buf);
        ret = len;
    }
    
    return ret;
}

/** @fn
  * @brief	    创建/proc下的chrdev_proc节点
  * @param[in]  
  * @param[out]
  * @return
  */

void chrdev_create_proc(void)
{
    struct proc_dir_entry  *entry = NULL;

    // 在/proc目录下创建一个名为chrdev_proc的文件，并默认具有全局可读权限
    entry = create_proc_entry("chrdev_proc", 666, NULL);
    if (entry)
    {
        entry->read_proc = chrdev_read_proc;
        entry->write_proc = chrdev_write_proc;
    }
    return;
}

/** @fn
  * @brief	    删除/proc下的chrdev_proc节点
  * @param[in]  
  * @param[out]
  * @return
  */
void chrdev_remove_proc(void)
{
    // 删除入口项
    remove_proc_entry("chrdev_proc", NULL);
    return;
}

