      
/**@file main.c
 * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief  实现字符设备常用接口open/close/read/write/ioctl以及驱动模块的加载初始
 * @brief  化及卸载清理 。
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
 
#include <linux/module.h>
#include "global.h"
#include "chrdev.h"
#include "file_opr.h"
#include "mem_proc.h"

// 内核模块通过module_param() 来传递命令行参数
INT32 g_chrdvr_nums = CHRDEV_NUMS;                     // 字符设备个数
static INT32 g_chrdev_major = CHRDEV_MAJOR;            // 主设备号 
static INT32 g_chrdev_minor = 0;                       // 次设备号
static INT8 * g_file_name = "task.txt";                // 操作的文件的名字
static INT32 g_mem_size = MEM_SIZE;                    // 设备内存的大小

module_param(g_chrdvr_nums, int, S_IRUGO);
module_param(g_chrdev_major, int, S_IRUGO);
module_param(g_chrdev_minor, int, S_IRUGO);
module_param(g_file_name, charp, S_IRUGO);
module_param(g_mem_size, int, S_IRUGO);

PMyCharDevice g_chrdev_devices;                         // 设备结构体指针，在chrdev_init_module 中分配空间

/** @fn
  * @brief	    open()系统调用函数接口
  * @param[in]  inode : 字符设备inode结构体指针
  * @param[out] filp : 存放设备结构体指针
  * @return	    0 : 成功
  */
int chrdev_open(struct inode *inode, struct file *filp)
{
    PMyCharDevice dev = NULL;
    dev = container_of(inode->i_cdev, MyCharDevice, cdev);
    filp->private_data = dev;                         // 将设备结构体指针赋值给文件私有数据指针
    return 0;
}

 /** @fn
  *  @brief	     release()系统调用函数接口，暂不包含任何内容
  *  @param[in]  inode : 字符设备inode结构体指针
  *  @param[out] filp : 存放设备结构体指针
  *  @return	 0 : 成功
  */
int chrdev_release(struct inode *inode, struct file *filp)
{
    return 0;
}

/** @fn
 *  @brief	    ioctl()系统调用函数接口
 *  @param[in]  filp : 暂时被用做存放设备结构体指针 cmd IOCTL命令编码 arg 命令参数
 *  @param[out]
 *  @return	    0 成功
 */
int chrdev_ioctl(struct inode *inodep, struct file *filp, unsigned 
                 int cmd, unsigned long arg)
{
    INT32 retval = 0;
    INT32 err = 0;
    PMyCharDevice dev = filp->private_data; //由chrdev_open()获得的设备结构体指针
    
    // 检查命令类型的有效性
    if (_IOC_TYPE(cmd) != CHRDEV_IOC_MAGIC) 
    {
        return -ENOTTY;
    }

    // 检查操作目录的权限的有效性
    if (_IOC_DIR(cmd) & _IOC_READ)
    {
        err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
    }
    else 
    {
        if (_IOC_DIR(cmd) & _IOC_WRITE)
        {
            err =  !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
        }
    }
    if (err)
    {
         return -EFAULT;
    }
    switch (cmd)
    {
		// 将设备buffer清零
        case CHRDEV_IOC_BUFCLEAR:          
            DBG_PRINT("[Success]Enter chrdev_ioctl() case : CHRDEV_IOC_BUFCLEAR!\n");
            if (down_interruptible(&dev->sem))
            {
                return  - ERESTARTSYS;
            }
            memset(dev->mem, 0, dev->mem_size);
            up(&dev->sem);                // 释放信号量

            DBG_PRINT("[Success]The buffer is clear to zero!\n");
            break;

        // 返回当前的设备缓存区数据总量值给用户层
        case CHRDEV_IOC_SIZE:
			retval = __put_user(dev->cur_size, (int __user *)arg);
            DBG_PRINT("[Success]Enter chrdev_ioctl() case : CHRDEV_IOC_SIZE!\n");
            return dev->cur_size;

        default:
            return  -ENOTTY;
    }
    return 0;
}
 
/** @fn
 *  @brief	   read()系统调用接口，实现对循环缓冲区的最大利用，缺点: 实现较麻烦，两次调用copy_to_user()
 *  @param[in]  filp : 存放设备结构的指针 buf : 用户空间请求读取字符串的指针
 *             size : 用户空间请求读取字符串长度 offp : 用户进行存取操作的位置，暂未实现
 *  @param[out] 
 *  @return	   成功读取的字符数 or 相应的错误码 
 */
ssize_t chrdev_read(struct file *filp, char __user *buf, size_t size,
                    loff_t *offp)
{
    size_t count = size;
    size_t to_end_count = 0;                // 当前读取位置到结尾之间的字符数
    size_t from_begin_count = 0;            // 开始位置到当前位置之间的字符数
    int ret = 0;
    PMyCharDevice dev = filp->private_data; // 由chrdev_open()获得的设备结构体指针

    if (down_interruptible(&dev->sem))
    {
        return  - ERESTARTSYS;
    }

    while(dev->p_read == dev->p_write)      // 无数据可读时，结束循环
    {
        up(&dev->sem);                      // 释放锁
        if(filp->f_flags & O_NONBLOCK)
        {
            return -EAGAIN;
        }
		
        DBG_PRINT("chrdev_read() : %s reading: going to sleep!\n", current->comm);
		
		// 修改task的状态为TASK_INTERRUPTIBLE，该进程将不会继续运行直到被唤醒
        if(wait_event_interruptible(dev->read_queue, (dev->p_read != dev->p_write)))
        {
            return -ERESTARTSYS;            // 有信号中断，通知fs层做相应处理
        }

        if(down_interruptible(&dev->sem))   // 首先获得信号量，防止竞争
        {
            return  - ERESTARTSYS;
        }

    }

    if(dev->p_read < dev->p_write)          // 数据就绪,可以进行读操作
    {
        // 读指针在写指针之后，一般情况
        count = min(count, (size_t)(dev->p_write - dev->p_read));
        if (copy_to_user(buf, (void*)(dev->p_read), count))
        {
            ret =  - EFAULT;
        }
        else
        {
            ret = count;
            DBG_PRINT("读指针在写指针之后的情况: Read %d byte(s) from %u !\n", count
                      , (UINT32)(dev->p_read - dev->mem));
            dev->p_read += count; 
            dev->cur_size -= count;        // 当前可读字符数减少
        }
    }
    else
    {
        // 读指针在写指针之前，此时，写指针已经回卷
        to_end_count = (size_t)(dev->mem_end - dev->p_read + 1); 
        from_begin_count = (size_t)(dev->p_write - dev->mem);
        if(count <= to_end_count)          // 缓存区末尾之前的数据足够
        {
            if (copy_to_user(buf, (void*)(dev->p_read), count))
            {
                ret =  - EFAULT;
            }
            else
            {
                ret = count;
                DBG_PRINT("缓存区末尾之前的数据足够 : read %d bytes(s) from %u \n", count
                          , (UINT32)(dev->p_read - dev->mem));
                dev->cur_size -= count;
                if(count == to_end_count)
                {
                    dev->p_read = dev->mem;  // 正好到达尾部，读完回卷指针
                }
                else
                {
                    dev->p_read += count;
                }
                
            }
        }
        else 
        {
			// 缓存区末尾之前的数据不足，回卷读指针，从缓存头开始再读取剩余的数据
            if (copy_to_user(buf, (void*)(dev->p_read), to_end_count))
            {
                ret =  - EFAULT;
            }
            else
            {
                DBG_PRINT(" 缓存区末尾之前的数据不足 : Read %d bytes(s) from %u \n", to_end_count
                          , (UINT32)(dev->p_read - dev->mem));
                ret = to_end_count;
            }

            dev->p_read = dev->mem;                // 回卷读指针

            count = min(count - to_end_count, from_begin_count);
            if (copy_to_user(buf + to_end_count, (void*)(dev->p_read), count))
            {
                ret =  - EFAULT;
            }
            else
            {
                DBG_PRINT("回卷读指针 : Read %d bytes(s) from %d \n", count
                          , (UINT32)(dev->p_read - dev->mem));
                ret += count ;
                dev->cur_size -= ret;
                dev->p_read += count;
            }
        }
    } 
	
    up(&dev->sem); // 释放信号量

    wake_up_interruptible(&dev->write_queue);

    return ret;
}

/** @fn
 *  @brief	    获取剩余可写入空间缓冲区的大小
 *  @param[in]  dev : 当前操作的设备结构指针
 *  @param[out] 
 *  @return	    剩余可写入空缓冲区的大小
 */
size_t get_freespace_num(PMyCharDevice dev)
{
    if(dev->p_read == dev->p_write)
    {
        return dev->mem_size - 1 ;           // 预留一个空间用来判定缓冲区是否变满
    }
    else
    {
        return (((dev->p_read + dev->mem_size - dev->p_write) % dev->mem_size) - 1 );
    }
}

/** @fn
  *	@brief	   write()系统调用接口，实现对循环缓冲区的最大利用，缺点: 实现较麻烦，两次调用copy_to_user()
  *	@param[in]	filp : 存放设备结构的指针 buf : 用户空间请求写入字符串的指针
  *			    size : 用户空间请求写入字符串长度 offp : 用户进行存取操作的位置，暂未实现
  *	@param[out] 
  *	@return    成功写入的字符数 or 相应的错误码 
  */
ssize_t chrdev_write(struct file *filp, const char __user *buf,
                     size_t size, loff_t *offp)
{
    size_t count = size;
    size_t freespace_num = 0;                // 当前剩余可写入的空缓冲区的大小
    size_t to_end_count = 0;                 // 当前读取位置到结尾之间的字符数
    size_t from_begin_write = 0;             // 开始位置到当前位置之间的字符数
    int ret = 0;

    PMyCharDevice dev = filp->private_data;  // 获得设备结构体指针

    // 获得信号量
    if (down_interruptible(&dev->sem))
    {
        return  - ERESTARTSYS;
    }
    
    
    while(0 == (freespace_num = get_freespace_num(dev))) // 缓存已满
    {
        up(&dev->sem);                      // 释放锁
        
        if(filp->f_flags & O_NONBLOCK)
        {
            return -EAGAIN;
        }
        DBG_PRINT("chrdev_write() : %s writing: going to sleep!\n", current->comm);
        if(wait_event_interruptible(dev->write_queue
                                    , ((freespace_num = get_freespace_num(dev)) != 0)))
        {
            return -ERESTARTSYS;            // 有信号中断，通知fs层做相应处理
        }
        if(down_interruptible(&dev->sem))   // 首先获得信号量，防止竞争
        {
            return  - ERESTARTSYS;
        }
    }

    freespace_num = get_freespace_num(dev);   
    count = min(count, freespace_num);       // 获取当前可以写入的最多的字符数

    // 有空间可用，接受数据的写入
    if(dev->p_read <= dev->p_write)
    {
        // 读指针在写指针之后
        to_end_count = (size_t)(dev->mem_end - dev->p_write + 1);
        if(count <= to_end_count)
        {
            // 写指针到尾的长度足够此次读取的长度
            if (copy_from_user(dev->p_write, buf, count))
            {
                ret = -EFAULT;
            }
            else
            {
                DBG_PRINT("写指针到尾的长度足够此次读取的长度 : written %d bytes(s) from %u\n", count
                          , (UINT32)(dev->p_write - dev->mem));
                if(count == to_end_count)
                {
                    dev->p_write = dev->mem;
                }
                else
                {
                    dev->p_write += count;
                }
                ret = count;
                dev->cur_size += count;
            }

        }
        else
        {
            // 写指针到结尾的长度足请求写入的长度，回卷后从开始再写入部分数据
            from_begin_write = count - to_end_count;
            if (copy_from_user(dev->p_write, buf, to_end_count))
            {
                ret = -EFAULT;
            }
            else
            {
                ret = to_end_count;
                DBG_PRINT("写指针到结尾的长度足请求写入的长度: written %d bytes(s) from %u\n", to_end_count
                          , (UINT32)(dev->p_write - dev->mem));
                dev->cur_size += to_end_count; 
                
            }
            dev->p_write = dev->mem; // 写指针回卷
            if (copy_from_user(dev->p_write, buf + to_end_count, from_begin_write))
            {
                ret = -EFAULT;
            }
            else
            {
                ret += from_begin_write;
                DBG_PRINT("写指针回卷 : written %d bytes(s) from %u\n", from_begin_write
                          , (UINT32)(dev->p_write - dev->mem));
                dev->p_write += from_begin_write;
                dev->cur_size += from_begin_write;
                
            }
        }
    }
    else
    {
        // 读指针在写指针之前，对于慢速读设备容易发生此种情况
        if (copy_from_user(dev->p_write, buf, count))
        {
            ret = -EFAULT;
        }
        else
        {
            DBG_PRINT("written %d bytes(s) from %u\n", count
                      , (UINT32)(dev->p_write - dev->mem));
            dev->p_write += count;
            dev->cur_size += count;
            ret = count;
            
        }
    }

    wake_up_interruptible(&dev->read_queue);  // 唤醒阻塞在read()和 select()上的读取者

    up(&dev->sem);                            //释放信号量

    return ret;
}

/** @fn
  *	@brief	    poll()轮巡接口调用，查询设备缓冲区的读写状态（可写、可读）
  *	@param[in]	filp : 存放设备结构的指针 wait : poll_table结构
  *	@param[out] 
  *	@return     空闲空间大小
  */
static UINT32 chrdev_poll(struct file *filp, poll_table *wait)
{
    PMyCharDevice dev = filp->private_data;
    UINT32 mask = 0;

    // 读写指针处于环形缓存区的相同位置时，不可读 ;
    // 写指针加1若为读指针，则缓存区已满，不可写
    down(&dev->sem);

    // 驱动程序向wait 添加一个等待队列
    poll_wait(filp, &dev->read_queue,  wait);
    poll_wait(filp, &dev->write_queue, wait);
    if (dev->p_read != dev->p_write) //
    {
        DBG_PRINT("[Success]chrdev_poll() can be read!\n");
        mask |= POLLIN | POLLRDNORM;    // 设备可读
    }
    if (get_freespace_num(dev) != 0)
    {
        DBG_PRINT("[Success]Enter chrdev_poll() can be write\n");
        mask |= POLLOUT | POLLWRNORM;   // 设备可写
    }
    up(&dev->sem);
    return mask;
}

/**@brief	定义相关的文件操作	  
  */
struct file_operations chrdev_fops = 
{
    .owner =    THIS_MODULE,
    .read =     chrdev_read,   // 从设备中读取数据
    .write =    chrdev_write,  // 向设备中写入数据
    .poll =     chrdev_poll,   // 查询设备缓冲区的读写状态
    .ioctl =    chrdev_ioctl,  // 执行ioctl命令码
    .open =     chrdev_open,   // 打开设备文件
    .release =  chrdev_release,// 释放设备文件
};

/** @fn
  *	@brief	    初始化并注册cdev 
  *	@param[in]	dev : 设备结构体指针 index : 设备索引编号
  *	@param[out] 
  *	@return
  */
void chrdev_setup_cdev(PMyCharDevice dev, INT32 index)
{
    INT32 err = 0;
    INT32 devno = MKDEV(g_chrdev_major, index);

    cdev_init(&dev->cdev, &chrdev_fops);
    dev->cdev.owner = THIS_MODULE;
    dev->cdev.ops = &chrdev_fops;
    dev->p_read = dev->mem;
    dev->p_write = dev->mem;
    dev->mem_size = g_mem_size;
    dev->mem_end = dev->mem + dev->mem_size - 1;
    dev->cur_size = 0;
    err = cdev_add(&dev->cdev, devno, 1);
    if (err)
    {
        DBG_PRINT("Error %d adding LED%d", err, index);
    }
    return;
}

/** @fn
  *	@brief	    将设备结构中的相关参数置空或清零,释放空间
  *	@param[in]	dev : 设备结构体指针
  *	@param[out] 
  *	@return     0 : 成功 else : 失败
  */
INT32 chrdev_clear(PMyCharDevice dev)
{
    if (down_interruptible(&dev->sem))    //获得信号量
    {
        return  - ERESTARTSYS;
    }

    dev->cur_size = 0;
    if(dev->mem)
    {
        //kfree(dev->mem);
        //vfree(dev->mem);
        free_pages((unsigned long)(dev->mem), 10);
    }
    up(&dev->sem);                        //释放信号量

    return 0;
}

/** @fn
  *	@brief	    模块清除函数
  *	@param[in]
  *	@param[out] 
  *	@return
  */
void __exit chrdev_release_module(void)
{
    int i = 0;
    dev_t devno = MKDEV(g_chrdev_major, g_chrdev_minor);

    DBG_PRINT("Enter chrdev_exit()~~~\n" );

    //  释放之前kmalloc()申请的结构体变量
    if (g_chrdev_devices != NULL) 
    {
        for (i = 0; i < g_chrdvr_nums; i++) 
        {
            file_write(g_file_name, g_chrdev_devices[i].mem, g_chrdev_devices[i].mem_size);
            DBG_PRINT("the data is :%s\n", g_chrdev_devices[i].mem);
            chrdev_clear(g_chrdev_devices + i);
            cdev_del(&g_chrdev_devices[i].cdev);
        }
        kfree(g_chrdev_devices);
    }

#ifdef DEBUG_PROC
    chrdev_remove_proc();
#endif
    unregister_chrdev_region(devno, g_chrdvr_nums);
    return;
}

 /** @fn
  *	@brief	     模块初始化函数 
  *	@param[in]
  *	@param[out]  
  *	@return       0：成功；相关错误码：失败
  */
int __init chrdev_init_module(void)
{
    INT32 ret = 0;
    int i = 0;
    dev_t dev = 0; 

    DBG_PRINT( "[Success]Enter chrdev_init_module()!\n" );
    if (g_chrdev_major)   
    {
		// 静态申请
        dev = MKDEV(g_chrdev_major, g_chrdev_minor);                
        ret = register_chrdev_region(dev, g_chrdvr_nums, "chrdev"); 
    }
    else
    {
        // 动态分配
        ret = alloc_chrdev_region(&dev, g_chrdev_minor, g_chrdvr_nums,
                                  "chrdev");
        g_chrdev_major = MAJOR(dev);
    }

    // 分配设备编号失败
    if (ret < 0)
    {
        DBG_PRINT("[Fail]Can't get chrdev_major %d\n", g_chrdev_major);
        return ret;
    }

    g_chrdev_devices = kmalloc(g_chrdvr_nums * sizeof(MyCharDevice), GFP_KERNEL);
    if (NULL == g_chrdev_devices)
    {
        ret = -ENOMEM;
        goto fail;     // 失败统一处理
    }

    memset(g_chrdev_devices, 0, g_chrdvr_nums * sizeof(MyCharDevice));

    // 循环初始化每一个设备
    for (i = 0; i < g_chrdvr_nums; i++)
    {
        g_chrdev_devices[i].mem = (void *)__get_free_pages(GFP_KERNEL, 10);
        if(!g_chrdev_devices[i].mem)
        {
            DBG_PRINT("[Fail]__get_free_pages for g_chrdev_devices[%d].mem failure!\n", i);
            goto fail;
        }

        // 初始化申请到的空间
        memset(g_chrdev_devices[i].mem, 0,g_mem_size);
        init_MUTEX(&g_chrdev_devices[i].sem);
        init_waitqueue_head(&g_chrdev_devices[i].read_queue);
        init_waitqueue_head(&g_chrdev_devices[i].write_queue);
        DBG_PRINT("Start to read File from Kernel!\n"); 
        ret = file_read(g_file_name, g_mem_size, g_chrdev_devices[i].mem);
        if(ret != 0)
        {
            DBG_PRINT("[Fail]file_read() failed\n");
            goto fail;
        }
        chrdev_setup_cdev(&g_chrdev_devices[i], i);
    }

// 调用proc接口
#ifdef PROC_DEBUG
    chrdev_create_proc();
#endif

    DBG_PRINT("[Success]chrdev_init_module() finished!\n" );
    return 0; 

    fail:
    // 释放申请的资源
    chrdev_release_module();
    DBG_PRINT("chrdev_init() failed！\n" );
    return ret;

}

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("liuboyf1"); 
MODULE_DESCRIPTION("My first char device driver"); 

module_init(chrdev_init_module);
module_exit(chrdev_release_module);
