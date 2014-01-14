/*	@file ctest.c
 *	@note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *	@brief 简单的字符型驱动程序
 *	@author qinhuatao
 *	@date 2011/10/26
 *	@note
 *
 */
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/poll.h>
#include <linux/proc_fs.h>

#include "kdbg.h"

#define CTEST_SIZE	0x400000	/* 全局mem最大4M字节 */
#define CTEST_DATA_SIZE CTEST_SIZE - 1	/* 数据长度 */
#define PROC_BUFF_SIZE 0x100	/* 定义proc接口缓冲 */ 
#define CTEST_MEM_CLEAR 0x1  /* 清0全局内存的长度 */
#define CTEST_MAJOR 200    /* 预设的ctest的主设备号 */

static int ctest_major = CTEST_MAJOR;

/*ctest设备结构体*/
struct ctest_dev                                     
{                                                        
	struct cdev cdev;				/*cdev结构体*/                       
	char *r_index;
	char *w_index;    
	char *mem;						/* 全局内存 */
	char proc_buff[PROC_BUFF_SIZE];	/* proc缓冲 */
	struct semaphore sem;			/* 并发控制用的信号量 */           
	wait_queue_head_t r_wait;		/* 阻塞读用的等待队列头 */     
	wait_queue_head_t w_wait;		/* 阻塞写用的等待队列头 */     
};

struct ctest_dev *ctest_devp; /* 设备结构体指针 */
static struct proc_dir_entry *ctest_proc_entry; /* proc结构体指针 */

/**	@fn	int ctest_open(struct inode *inode, struct file *filp)
 *	@brief	将设备结构体指针赋值给文件私有数据指针
 *	@param struct inode *inode 文件节点指针
 *	@param struct file *filp 设备文件指针
 *	@return	0:正常返回，-1:错误返回
 */
int ctest_open(struct inode *inode, struct file *filp)
{
  /* 将设备结构体指针赋值给文件私有数据指针 */
	filp->private_data = ctest_devp;
	return 0;
}

/**	@fn	int ctest_open(struct inode *inode, struct file *filp)
 *	@brief	用于释放申请的资源
 *	@param struct inode *inode 文件节点指针
 *	@param struct file *filp 设备文件指针
 *	@return	0:正常返回，-1:错误返回
 */
int ctest_release(struct inode *inode, struct file *filp)
{
	return 0;
}

/**	@fn	static int ctest_ioctl(struct inode *inodep, struct file *filp, unsigned
	int cmd, unsigned long arg)
 *	@brief	ioctl接口
 *	@param struct inode *inodep 文件节点指针
 *	@param struct file *filp 设备文件指针
 *	@param unsigned int cmd 命令
 *	@param unsigned long arg 命令参数
 *	@return	cmd:正常返回，其他出错:错误返回
 */
static int ctest_ioctl(struct inode *inodep, struct file *filp, unsigned
  int cmd, unsigned long arg)
{
	/*获得设备结构体指针*/
	struct ctest_dev *dev = filp->private_data; 
	dbg_prt_line;
	switch (cmd)
	{
	case CTEST_MEM_CLEAR:
		down(&dev->sem); 
		dev->r_index = dev->mem;
		dev->w_index = dev->mem;
		up(&dev->sem);          
		dev_info("\nkernel:ctest_mem is set to zero\n");
	break;

	default:
		return  - EINVAL;
	}
	return cmd;
}

/**	@fn	static unsigned int ctest_poll(struct file *filp, poll_table *wait)
 *	@brief	轮训接口函数
 *	@param struct inode *inode 文件节点指针
 *	@param struct poll_table *wait 等待队列
 *	@return	mask 状态标示
 */
static unsigned int ctest_poll(struct file *filp, poll_table *wait)
{
	unsigned int mask = 0;
	struct ctest_dev *dev = filp->private_data; /*获得设备结构体指针*/
  
	dbg_prt_line;
	down(&dev->sem);
  
	poll_wait(filp, &dev->r_wait, wait);
	poll_wait(filp, &dev->w_wait, wait);  
	/*缓冲区可读*/
	if (dev->r_index != dev->w_index)
	{
		mask |= POLLIN | POLLRDNORM; /*标示数据可获得*/
	}
	/* 缓冲区可写 */
	if (dev->w_index + 1 != dev->r_index)
	{
		mask |= POLLOUT | POLLWRNORM; /*标示数据可写入*/
	}
     
	up(&dev->sem);
	return mask;
}


/**	@fn	static ssize_t ctest_read(struct file *filp, char __user *buf, size_t count,
  loff_t *ppos)
 *	@brief	读取函数接口
 *	@param struct inode *inode 文件节点指针
 *	@param struct file *filp 文件指针
 *	@param size_t count 需要复制的字符数
 *	@param loff_t *ppos 文件指针偏移量
 *	@return	count 复制的字节数
 */
static ssize_t ctest_read(struct file *filp, char __user *buf, size_t count,
  loff_t *ppos)
{
	int ret, num, capability;
	struct ctest_dev *dev = filp->private_data; /* 获得设备结构体指针 */
	DECLARE_WAITQUEUE(wait, current); /* 定义等待队列 */

	down(&dev->sem); /* 获得信号量 */
	add_wait_queue(&dev->r_wait, &wait); /* 加入等待队列头 */

	/* 如果缓冲区空 */
	if (dev->r_index == dev->w_index)
	{
		dbg_prt_line;
		wake_up_interruptible(&dev->w_wait); /* 唤醒写等待队列 */

	
		if (filp->f_flags & O_NONBLOCK)
		{
			ret =  - EAGAIN;
			up(&dev->sem); 
			remove_wait_queue(&dev->w_wait, &wait); /* 移出等待队列 */
			return ret;
		} 
		set_current_state(TASK_INTERRUPTIBLE); /* 线程休眠 */
		up(&dev->sem);

		/* 调度其他线程 */
		schedule(); 

		#if 0
		/* 如果是因为信号唤醒 */
		if (signal_pending(current))
		{
			ret = - ERESTARTSYS;
			remove_wait_queue(&dev->w_wait, &wait); /* 移出等待队列 */
			set_current_state(TASK_RUNNING);
			return ret;
		}
		#endif
		
		down(&dev->sem);
	}
	dbg_prt_line;
	
	/* 计算复制长度 */
	capability = dev->w_index - dev->r_index;
	if (capability < 0)
	{
		capability += CTEST_SIZE;
	}
	num = dev->mem - dev->r_index + CTEST_SIZE;
	num = num < capability ? num : capability;
	count = (count <= (u32)num) ? count : num;

	/* 复制 */
	copy_to_user(buf, dev->r_index, count);
	wake_up_interruptible(&dev->w_wait); /* 唤醒写等待队列 */
	ret = count;
  
	/* 修正指针 */
	dev->r_index += count;
	if ((dev->r_index - dev->mem) == CTEST_SIZE)
	{
		dev->r_index = dev->mem;
	}
	else if ((dev->r_index - dev->mem) > CTEST_SIZE)
	{
		dbg_prt_line;
		dev_err("r_index > CTEST_SIZE\n");
	}
	//dev_dbg("r_index = %d\n",dev->r_index - dev->mem);
	up(&dev->sem); 
	remove_wait_queue(&dev->w_wait, &wait); /* 移出等待队列 */
	set_current_state(TASK_RUNNING);
	return ret;
}


/**	static ssize_t ctest_write(struct file *filp, const char __user *buf,
  size_t count, loff_t *ppos)
 *	@brief	写函数接口
 *	@param struct inode *inode 文件节点指针
 *	@param struct file *filp 文件指针
 *	@param size_t count 需要复制的字符数
 *	@param loff_t *ppos 文件指针偏移量
 *	@return	count 返回复制的字节数
 */
static ssize_t ctest_write(struct file *filp, const char __user *buf,
  size_t count, loff_t *ppos)
{
	/* 获得设备结构体指针 */
	struct ctest_dev *dev = filp->private_data; 
	int ret, num, capability;
	DECLARE_WAITQUEUE(wait, current); 

	down(&dev->sem); 
	add_wait_queue(&dev->w_wait, &wait); /* 进入写等待队列头 */

	/* 如果缓冲区满 */
	if (dev->w_index + 1 == dev->r_index || CTEST_DATA_SIZE == dev->w_index - dev->r_index)
	{
		dbg_prt_line;
		wake_up_interruptible(&dev->r_wait); /* 唤醒读等待队列 */

		/* 如果是非阻塞访问 */
		if (filp->f_flags & O_NONBLOCK)
		{
			ret =  - EAGAIN;
			up(&dev->sem); 
			remove_wait_queue(&dev->w_wait, &wait); /* 移出等待队列 */
			return ret;
		} 
		set_current_state(TASK_INTERRUPTIBLE); /* 线程休眠 */
		up(&dev->sem);

		schedule(); /* 调度其他线程 */
		
		#if 0
		/* 如果为信号唤醒 */
		if (signal_pending(current))
		{
			ret =  - ERESTARTSYS;
			remove_wait_queue(&dev->w_wait, &wait); /* 移出等待队列 */
			set_current_state(TASK_RUNNING);
			return ret;
		}
		#endif
		
		down(&dev->sem); 
	}
  
	dbg_prt_line;
	
	/* 计算复制长度 */
	capability = dev->r_index - dev->w_index;
	if (capability <= 0)
	{
		capability  += CTEST_DATA_SIZE;
	}
	else
	{
		capability -= 1;
	}
	num = dev->mem - dev->w_index + CTEST_SIZE;
	num = num < capability ? num : capability;
	count = (count < (unsigned)num) ? count : (unsigned)num;
  
	copy_from_user(dev->w_index, buf, count);
  
	/* 修正指针 */
	dev->w_index += count;
	if ((dev->w_index - dev->mem) == CTEST_SIZE)
	{
		dev->w_index = dev->mem;
	}
	else if ((dev->w_index - dev->mem) > CTEST_SIZE)
	{
		dbg_prt_line;
		dev_err("error :w_index > size\n");
	}

	/* 唤醒读等待队列 */
	wake_up_interruptible(&dev->r_wait);
	ret = count;
	//dev_dbg("w_index = %d\n",dev->w_index - dev->mem);
	up(&dev->sem); //释放信号量
	remove_wait_queue(&dev->w_wait, &wait); /* 移出等待队列 */
	set_current_state(TASK_RUNNING); /* 设置线程运行 */
	return ret;
}



/**	static ssize_t ctest_proc_read(char *page, char **start, off_t off, int count,
  int*eof, void *data)
 *	@brief	读函数接口
 *	@param char *page 页指针
 *	@param char **start 其实地址指针
 *	@param off_t off 偏移量
 *	@param int count 复制字节数
 *	@return	len 实际复制的字节数 
 */
static ssize_t ctest_proc_read(char *page, char **start, off_t off, int count,
  int*eof, void *data)
{
	int len;
	dbg_prt_line;
	if (off > 0)
	{
		*eof = 1;
		return 0;
	}
	len = sprintf(page, "%s\n",ctest_devp->proc_buff);
	return len;
}

/**	static ssize_t ctest_proc_write(struct file *filp, const char __user *buff, unsigned
	long len, void *data)
 *	@brief	写proc函数接口
 *	@param char *page 页指针
 *	@param char **start 其实地址指针
 *	@param off_t off 偏移量
 *	@param int count 复制字节数
 *	@return	len 实际复制的字节数 
 */
static ssize_t ctest_proc_write(struct file *filp, const char __user *buff, unsigned
	long len, void *data)
{

	//unsigned long new;
	int count;
	dbg_prt_line;
	count = (PROC_BUFF_SIZE < len) ? PROC_BUFF_SIZE : len;
	copy_from_user(ctest_devp->proc_buff, buff, count);
	return count;
}

/**	static int ctest_proc_init(void)
 *	@brief	proc接口初始化
 *	@return	len 实际复制的字节数 
 */
static int ctest_proc_init(void)
{
	dbg_prt_line;
	ctest_proc_entry = create_proc_entry("ctest_proc", 0666, NULL); //创建/proc
	if (ctest_proc_entry == NULL)
	{
		dev_info("Couldn't create proc entry\n");
		return - ENOMEM;
	}
	else
	{
		ctest_proc_entry->read_proc = ctest_proc_read;
		ctest_proc_entry->write_proc = ctest_proc_write;
		ctest_proc_entry->owner = THIS_MODULE;
	}
	return 0;

}

/**	static int ctest_proc_init(void)
 *	@brief	proc接口卸载
 *	@return	len 实际复制的字节数 
 */
static void ctest_proc_exit(void)
{
	dbg_prt_line;
	remove_proc_entry("ctest_proc", &proc_root); //撤销/proc
}






/**	static const struct file_operations ctest_fops
 *	@brief	文件操作结构体
 */
static const struct file_operations ctest_fops =
{
	.owner = THIS_MODULE,
	.read = ctest_read,
	.write = ctest_write,
	.ioctl = ctest_ioctl,
	.poll = ctest_poll,
	.open = ctest_open,
	.release = ctest_release,
};

/**	static void ctest_setup_cdev(struct ctest_dev *dev, int index)
 *	@brief	写proc函数接口
 *	@param struct ctest_dev *dev 设备结构体指针
 *	@param int index 子设备号
 *	@return	；
 */
static void ctest_setup_cdev(struct ctest_dev *dev, int index)
{
	int err, devno = MKDEV(ctest_major, index);
	dbg_prt_line;

	cdev_init(&dev->cdev, &ctest_fops);
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops = &ctest_fops;
  
	err = cdev_add(&dev->cdev, devno, 1);
	if (err)
	{
		dev_notice("Error adding dev%d\n", index);
	}
  
	err = ctest_proc_init();
	if (err)
	{
		dev_info("init ctest proc error\n");
	}

	dev_info("setup ctest successfully!\n");
	return;
}

/**	static int ctest_init(void)
 *	@brief	设备模块加载
 *	@return	0 正常；负值 异常
 */
int ctest_init(void)
{
	int ret;
	dev_t devno = MKDEV(ctest_major, 0);
	dbg_prt_line;

	/* 申请设备号*/
	if (ctest_major)
        ret = register_chrdev_region(devno, 1, "ctest");
	else  /* 动态申请设备号 */
	{
		ret = alloc_chrdev_region(&devno, 0, 1, "ctest");
		ctest_major = MAJOR(devno);
	}
	if (ret < 0)
	{
		return ret;
	}
	/* 动态申请设备结构体的内存 */
	ctest_devp = kmalloc(sizeof(struct ctest_dev), GFP_KERNEL);
	if (!ctest_devp)    /*申请失败*/
	{
		ret =  - ENOMEM;
		unregister_chrdev_region(devno, 1);
		return ret;
	}
	memset(ctest_devp, 0, sizeof(struct ctest_dev));

	ctest_devp->mem = (char *)__get_free_pages(GFP_KERNEL, 10);
    if (!(ctest_devp->mem))
    {
        dev_err("can't get device memory\n");
        ret =  - ENOMEM;
        unregister_chrdev_region(devno, 1);
        return ret;
    }

    init_MUTEX(&ctest_devp->sem);   /* 初始化信号量 */
    init_waitqueue_head(&ctest_devp->r_wait); /* 初始化读等待队列头 */
    init_waitqueue_head(&ctest_devp->w_wait); /* 初始化写等待队列头 */
	ctest_devp->w_index = ctest_devp->mem;
	ctest_devp->r_index = ctest_devp->mem;
  
	ctest_setup_cdev(ctest_devp, 0);
  
	return 0;
}
/**	static int ctest_init(void)
 *	@brief	模块卸载函数
 *	@return	0 正常；负值 异常
 */
void ctest_exit(void)
{
	dbg_prt_line;

	ctest_proc_exit();
	free_pages((unsigned long) ctest_devp->mem,10);
	cdev_del(&(ctest_devp->cdev));   /* 注销cdev */
	kfree(ctest_devp);     /* 释放设备结构体内存 */
	unregister_chrdev_region(MKDEV(ctest_major, 0), 1); /* 释放设备号 */
  
	dev_info("remove ctest successfully!\n");
}

MODULE_AUTHOR("Qin Huatao");
MODULE_LICENSE("Dual BSD/GPL");

module_param(ctest_major, int, S_IRUGO);

module_init(ctest_init);
module_exit(ctest_exit);
