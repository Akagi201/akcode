      
/**@file main.c
 * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief  ʵ���ַ��豸���ýӿ�open/close/read/write/ioctl�Լ�����ģ��ļ��س�ʼ
 * @brief  ����ж������ ��
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

// �ں�ģ��ͨ��module_param() �����������в���
INT32 g_chrdvr_nums = CHRDEV_NUMS;                     // �ַ��豸����
static INT32 g_chrdev_major = CHRDEV_MAJOR;            // ���豸�� 
static INT32 g_chrdev_minor = 0;                       // ���豸��
static INT8 * g_file_name = "task.txt";                // �������ļ�������
static INT32 g_mem_size = MEM_SIZE;                    // �豸�ڴ�Ĵ�С

module_param(g_chrdvr_nums, int, S_IRUGO);
module_param(g_chrdev_major, int, S_IRUGO);
module_param(g_chrdev_minor, int, S_IRUGO);
module_param(g_file_name, charp, S_IRUGO);
module_param(g_mem_size, int, S_IRUGO);

PMyCharDevice g_chrdev_devices;                         // �豸�ṹ��ָ�룬��chrdev_init_module �з���ռ�

/** @fn
  * @brief	    open()ϵͳ���ú����ӿ�
  * @param[in]  inode : �ַ��豸inode�ṹ��ָ��
  * @param[out] filp : ����豸�ṹ��ָ��
  * @return	    0 : �ɹ�
  */
int chrdev_open(struct inode *inode, struct file *filp)
{
    PMyCharDevice dev = NULL;
    dev = container_of(inode->i_cdev, MyCharDevice, cdev);
    filp->private_data = dev;                         // ���豸�ṹ��ָ�븳ֵ���ļ�˽������ָ��
    return 0;
}

 /** @fn
  *  @brief	     release()ϵͳ���ú����ӿڣ��ݲ������κ�����
  *  @param[in]  inode : �ַ��豸inode�ṹ��ָ��
  *  @param[out] filp : ����豸�ṹ��ָ��
  *  @return	 0 : �ɹ�
  */
int chrdev_release(struct inode *inode, struct file *filp)
{
    return 0;
}

/** @fn
 *  @brief	    ioctl()ϵͳ���ú����ӿ�
 *  @param[in]  filp : ��ʱ����������豸�ṹ��ָ�� cmd IOCTL������� arg �������
 *  @param[out]
 *  @return	    0 �ɹ�
 */
int chrdev_ioctl(struct inode *inodep, struct file *filp, unsigned 
                 int cmd, unsigned long arg)
{
    INT32 retval = 0;
    INT32 err = 0;
    PMyCharDevice dev = filp->private_data; //��chrdev_open()��õ��豸�ṹ��ָ��
    
    // ����������͵���Ч��
    if (_IOC_TYPE(cmd) != CHRDEV_IOC_MAGIC) 
    {
        return -ENOTTY;
    }

    // ������Ŀ¼��Ȩ�޵���Ч��
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
		// ���豸buffer����
        case CHRDEV_IOC_BUFCLEAR:          
            DBG_PRINT("[Success]Enter chrdev_ioctl() case : CHRDEV_IOC_BUFCLEAR!\n");
            if (down_interruptible(&dev->sem))
            {
                return  - ERESTARTSYS;
            }
            memset(dev->mem, 0, dev->mem_size);
            up(&dev->sem);                // �ͷ��ź���

            DBG_PRINT("[Success]The buffer is clear to zero!\n");
            break;

        // ���ص�ǰ���豸��������������ֵ���û���
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
 *  @brief	   read()ϵͳ���ýӿڣ�ʵ�ֶ�ѭ����������������ã�ȱ��: ʵ�ֽ��鷳�����ε���copy_to_user()
 *  @param[in]  filp : ����豸�ṹ��ָ�� buf : �û��ռ������ȡ�ַ�����ָ��
 *             size : �û��ռ������ȡ�ַ������� offp : �û����д�ȡ������λ�ã���δʵ��
 *  @param[out] 
 *  @return	   �ɹ���ȡ���ַ��� or ��Ӧ�Ĵ����� 
 */
ssize_t chrdev_read(struct file *filp, char __user *buf, size_t size,
                    loff_t *offp)
{
    size_t count = size;
    size_t to_end_count = 0;                // ��ǰ��ȡλ�õ���β֮����ַ���
    size_t from_begin_count = 0;            // ��ʼλ�õ���ǰλ��֮����ַ���
    int ret = 0;
    PMyCharDevice dev = filp->private_data; // ��chrdev_open()��õ��豸�ṹ��ָ��

    if (down_interruptible(&dev->sem))
    {
        return  - ERESTARTSYS;
    }

    while(dev->p_read == dev->p_write)      // �����ݿɶ�ʱ������ѭ��
    {
        up(&dev->sem);                      // �ͷ���
        if(filp->f_flags & O_NONBLOCK)
        {
            return -EAGAIN;
        }
		
        DBG_PRINT("chrdev_read() : %s reading: going to sleep!\n", current->comm);
		
		// �޸�task��״̬ΪTASK_INTERRUPTIBLE���ý��̽������������ֱ��������
        if(wait_event_interruptible(dev->read_queue, (dev->p_read != dev->p_write)))
        {
            return -ERESTARTSYS;            // ���ź��жϣ�֪ͨfs������Ӧ����
        }

        if(down_interruptible(&dev->sem))   // ���Ȼ���ź�������ֹ����
        {
            return  - ERESTARTSYS;
        }

    }

    if(dev->p_read < dev->p_write)          // ���ݾ���,���Խ��ж�����
    {
        // ��ָ����дָ��֮��һ�����
        count = min(count, (size_t)(dev->p_write - dev->p_read));
        if (copy_to_user(buf, (void*)(dev->p_read), count))
        {
            ret =  - EFAULT;
        }
        else
        {
            ret = count;
            DBG_PRINT("��ָ����дָ��֮������: Read %d byte(s) from %u !\n", count
                      , (UINT32)(dev->p_read - dev->mem));
            dev->p_read += count; 
            dev->cur_size -= count;        // ��ǰ�ɶ��ַ�������
        }
    }
    else
    {
        // ��ָ����дָ��֮ǰ����ʱ��дָ���Ѿ��ؾ�
        to_end_count = (size_t)(dev->mem_end - dev->p_read + 1); 
        from_begin_count = (size_t)(dev->p_write - dev->mem);
        if(count <= to_end_count)          // ������ĩβ֮ǰ�������㹻
        {
            if (copy_to_user(buf, (void*)(dev->p_read), count))
            {
                ret =  - EFAULT;
            }
            else
            {
                ret = count;
                DBG_PRINT("������ĩβ֮ǰ�������㹻 : read %d bytes(s) from %u \n", count
                          , (UINT32)(dev->p_read - dev->mem));
                dev->cur_size -= count;
                if(count == to_end_count)
                {
                    dev->p_read = dev->mem;  // ���õ���β��������ؾ�ָ��
                }
                else
                {
                    dev->p_read += count;
                }
                
            }
        }
        else 
        {
			// ������ĩβ֮ǰ�����ݲ��㣬�ؾ��ָ�룬�ӻ���ͷ��ʼ�ٶ�ȡʣ�������
            if (copy_to_user(buf, (void*)(dev->p_read), to_end_count))
            {
                ret =  - EFAULT;
            }
            else
            {
                DBG_PRINT(" ������ĩβ֮ǰ�����ݲ��� : Read %d bytes(s) from %u \n", to_end_count
                          , (UINT32)(dev->p_read - dev->mem));
                ret = to_end_count;
            }

            dev->p_read = dev->mem;                // �ؾ��ָ��

            count = min(count - to_end_count, from_begin_count);
            if (copy_to_user(buf + to_end_count, (void*)(dev->p_read), count))
            {
                ret =  - EFAULT;
            }
            else
            {
                DBG_PRINT("�ؾ��ָ�� : Read %d bytes(s) from %d \n", count
                          , (UINT32)(dev->p_read - dev->mem));
                ret += count ;
                dev->cur_size -= ret;
                dev->p_read += count;
            }
        }
    } 
	
    up(&dev->sem); // �ͷ��ź���

    wake_up_interruptible(&dev->write_queue);

    return ret;
}

/** @fn
 *  @brief	    ��ȡʣ���д��ռ仺�����Ĵ�С
 *  @param[in]  dev : ��ǰ�������豸�ṹָ��
 *  @param[out] 
 *  @return	    ʣ���д��ջ������Ĵ�С
 */
size_t get_freespace_num(PMyCharDevice dev)
{
    if(dev->p_read == dev->p_write)
    {
        return dev->mem_size - 1 ;           // Ԥ��һ���ռ������ж��������Ƿ����
    }
    else
    {
        return (((dev->p_read + dev->mem_size - dev->p_write) % dev->mem_size) - 1 );
    }
}

/** @fn
  *	@brief	   write()ϵͳ���ýӿڣ�ʵ�ֶ�ѭ����������������ã�ȱ��: ʵ�ֽ��鷳�����ε���copy_to_user()
  *	@param[in]	filp : ����豸�ṹ��ָ�� buf : �û��ռ�����д���ַ�����ָ��
  *			    size : �û��ռ�����д���ַ������� offp : �û����д�ȡ������λ�ã���δʵ��
  *	@param[out] 
  *	@return    �ɹ�д����ַ��� or ��Ӧ�Ĵ����� 
  */
ssize_t chrdev_write(struct file *filp, const char __user *buf,
                     size_t size, loff_t *offp)
{
    size_t count = size;
    size_t freespace_num = 0;                // ��ǰʣ���д��Ŀջ������Ĵ�С
    size_t to_end_count = 0;                 // ��ǰ��ȡλ�õ���β֮����ַ���
    size_t from_begin_write = 0;             // ��ʼλ�õ���ǰλ��֮����ַ���
    int ret = 0;

    PMyCharDevice dev = filp->private_data;  // ����豸�ṹ��ָ��

    // ����ź���
    if (down_interruptible(&dev->sem))
    {
        return  - ERESTARTSYS;
    }
    
    
    while(0 == (freespace_num = get_freespace_num(dev))) // ��������
    {
        up(&dev->sem);                      // �ͷ���
        
        if(filp->f_flags & O_NONBLOCK)
        {
            return -EAGAIN;
        }
        DBG_PRINT("chrdev_write() : %s writing: going to sleep!\n", current->comm);
        if(wait_event_interruptible(dev->write_queue
                                    , ((freespace_num = get_freespace_num(dev)) != 0)))
        {
            return -ERESTARTSYS;            // ���ź��жϣ�֪ͨfs������Ӧ����
        }
        if(down_interruptible(&dev->sem))   // ���Ȼ���ź�������ֹ����
        {
            return  - ERESTARTSYS;
        }
    }

    freespace_num = get_freespace_num(dev);   
    count = min(count, freespace_num);       // ��ȡ��ǰ����д��������ַ���

    // �пռ���ã��������ݵ�д��
    if(dev->p_read <= dev->p_write)
    {
        // ��ָ����дָ��֮��
        to_end_count = (size_t)(dev->mem_end - dev->p_write + 1);
        if(count <= to_end_count)
        {
            // дָ�뵽β�ĳ����㹻�˴ζ�ȡ�ĳ���
            if (copy_from_user(dev->p_write, buf, count))
            {
                ret = -EFAULT;
            }
            else
            {
                DBG_PRINT("дָ�뵽β�ĳ����㹻�˴ζ�ȡ�ĳ��� : written %d bytes(s) from %u\n", count
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
            // дָ�뵽��β�ĳ���������д��ĳ��ȣ��ؾ��ӿ�ʼ��д�벿������
            from_begin_write = count - to_end_count;
            if (copy_from_user(dev->p_write, buf, to_end_count))
            {
                ret = -EFAULT;
            }
            else
            {
                ret = to_end_count;
                DBG_PRINT("дָ�뵽��β�ĳ���������д��ĳ���: written %d bytes(s) from %u\n", to_end_count
                          , (UINT32)(dev->p_write - dev->mem));
                dev->cur_size += to_end_count; 
                
            }
            dev->p_write = dev->mem; // дָ��ؾ�
            if (copy_from_user(dev->p_write, buf + to_end_count, from_begin_write))
            {
                ret = -EFAULT;
            }
            else
            {
                ret += from_begin_write;
                DBG_PRINT("дָ��ؾ� : written %d bytes(s) from %u\n", from_begin_write
                          , (UINT32)(dev->p_write - dev->mem));
                dev->p_write += from_begin_write;
                dev->cur_size += from_begin_write;
                
            }
        }
    }
    else
    {
        // ��ָ����дָ��֮ǰ���������ٶ��豸���׷����������
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

    wake_up_interruptible(&dev->read_queue);  // ����������read()�� select()�ϵĶ�ȡ��

    up(&dev->sem);                            //�ͷ��ź���

    return ret;
}

/** @fn
  *	@brief	    poll()��Ѳ�ӿڵ��ã���ѯ�豸�������Ķ�д״̬����д���ɶ���
  *	@param[in]	filp : ����豸�ṹ��ָ�� wait : poll_table�ṹ
  *	@param[out] 
  *	@return     ���пռ��С
  */
static UINT32 chrdev_poll(struct file *filp, poll_table *wait)
{
    PMyCharDevice dev = filp->private_data;
    UINT32 mask = 0;

    // ��дָ�봦�ڻ��λ���������ͬλ��ʱ�����ɶ� ;
    // дָ���1��Ϊ��ָ�룬�򻺴�������������д
    down(&dev->sem);

    // ����������wait ���һ���ȴ�����
    poll_wait(filp, &dev->read_queue,  wait);
    poll_wait(filp, &dev->write_queue, wait);
    if (dev->p_read != dev->p_write) //
    {
        DBG_PRINT("[Success]chrdev_poll() can be read!\n");
        mask |= POLLIN | POLLRDNORM;    // �豸�ɶ�
    }
    if (get_freespace_num(dev) != 0)
    {
        DBG_PRINT("[Success]Enter chrdev_poll() can be write\n");
        mask |= POLLOUT | POLLWRNORM;   // �豸��д
    }
    up(&dev->sem);
    return mask;
}

/**@brief	������ص��ļ�����	  
  */
struct file_operations chrdev_fops = 
{
    .owner =    THIS_MODULE,
    .read =     chrdev_read,   // ���豸�ж�ȡ����
    .write =    chrdev_write,  // ���豸��д������
    .poll =     chrdev_poll,   // ��ѯ�豸�������Ķ�д״̬
    .ioctl =    chrdev_ioctl,  // ִ��ioctl������
    .open =     chrdev_open,   // ���豸�ļ�
    .release =  chrdev_release,// �ͷ��豸�ļ�
};

/** @fn
  *	@brief	    ��ʼ����ע��cdev 
  *	@param[in]	dev : �豸�ṹ��ָ�� index : �豸�������
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
  *	@brief	    ���豸�ṹ�е���ز����ÿջ�����,�ͷſռ�
  *	@param[in]	dev : �豸�ṹ��ָ��
  *	@param[out] 
  *	@return     0 : �ɹ� else : ʧ��
  */
INT32 chrdev_clear(PMyCharDevice dev)
{
    if (down_interruptible(&dev->sem))    //����ź���
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
    up(&dev->sem);                        //�ͷ��ź���

    return 0;
}

/** @fn
  *	@brief	    ģ���������
  *	@param[in]
  *	@param[out] 
  *	@return
  */
void __exit chrdev_release_module(void)
{
    int i = 0;
    dev_t devno = MKDEV(g_chrdev_major, g_chrdev_minor);

    DBG_PRINT("Enter chrdev_exit()~~~\n" );

    //  �ͷ�֮ǰkmalloc()����Ľṹ�����
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
  *	@brief	     ģ���ʼ������ 
  *	@param[in]
  *	@param[out]  
  *	@return       0���ɹ�����ش����룺ʧ��
  */
int __init chrdev_init_module(void)
{
    INT32 ret = 0;
    int i = 0;
    dev_t dev = 0; 

    DBG_PRINT( "[Success]Enter chrdev_init_module()!\n" );
    if (g_chrdev_major)   
    {
		// ��̬����
        dev = MKDEV(g_chrdev_major, g_chrdev_minor);                
        ret = register_chrdev_region(dev, g_chrdvr_nums, "chrdev"); 
    }
    else
    {
        // ��̬����
        ret = alloc_chrdev_region(&dev, g_chrdev_minor, g_chrdvr_nums,
                                  "chrdev");
        g_chrdev_major = MAJOR(dev);
    }

    // �����豸���ʧ��
    if (ret < 0)
    {
        DBG_PRINT("[Fail]Can't get chrdev_major %d\n", g_chrdev_major);
        return ret;
    }

    g_chrdev_devices = kmalloc(g_chrdvr_nums * sizeof(MyCharDevice), GFP_KERNEL);
    if (NULL == g_chrdev_devices)
    {
        ret = -ENOMEM;
        goto fail;     // ʧ��ͳһ����
    }

    memset(g_chrdev_devices, 0, g_chrdvr_nums * sizeof(MyCharDevice));

    // ѭ����ʼ��ÿһ���豸
    for (i = 0; i < g_chrdvr_nums; i++)
    {
        g_chrdev_devices[i].mem = (void *)__get_free_pages(GFP_KERNEL, 10);
        if(!g_chrdev_devices[i].mem)
        {
            DBG_PRINT("[Fail]__get_free_pages for g_chrdev_devices[%d].mem failure!\n", i);
            goto fail;
        }

        // ��ʼ�����뵽�Ŀռ�
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

// ����proc�ӿ�
#ifdef PROC_DEBUG
    chrdev_create_proc();
#endif

    DBG_PRINT("[Success]chrdev_init_module() finished!\n" );
    return 0; 

    fail:
    // �ͷ��������Դ
    chrdev_release_module();
    DBG_PRINT("chrdev_init() failed��\n" );
    return ret;

}

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("liuboyf1"); 
MODULE_DESCRIPTION("My first char device driver"); 

module_init(chrdev_init_module);
module_exit(chrdev_release_module);
