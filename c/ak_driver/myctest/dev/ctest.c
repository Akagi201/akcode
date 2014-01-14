/*	@file ctest.c
 *	@note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *	@brief �򵥵��ַ�����������
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

#define CTEST_SIZE	0x400000	/* ȫ��mem���4M�ֽ� */
#define CTEST_DATA_SIZE CTEST_SIZE - 1	/* ���ݳ��� */
#define PROC_BUFF_SIZE 0x100	/* ����proc�ӿڻ��� */ 
#define CTEST_MEM_CLEAR 0x1  /* ��0ȫ���ڴ�ĳ��� */
#define CTEST_MAJOR 200    /* Ԥ���ctest�����豸�� */

static int ctest_major = CTEST_MAJOR;

/*ctest�豸�ṹ��*/
struct ctest_dev                                     
{                                                        
	struct cdev cdev;				/*cdev�ṹ��*/                       
	char *r_index;
	char *w_index;    
	char *mem;						/* ȫ���ڴ� */
	char proc_buff[PROC_BUFF_SIZE];	/* proc���� */
	struct semaphore sem;			/* ���������õ��ź��� */           
	wait_queue_head_t r_wait;		/* �������õĵȴ�����ͷ */     
	wait_queue_head_t w_wait;		/* ����д�õĵȴ�����ͷ */     
};

struct ctest_dev *ctest_devp; /* �豸�ṹ��ָ�� */
static struct proc_dir_entry *ctest_proc_entry; /* proc�ṹ��ָ�� */

/**	@fn	int ctest_open(struct inode *inode, struct file *filp)
 *	@brief	���豸�ṹ��ָ�븳ֵ���ļ�˽������ָ��
 *	@param struct inode *inode �ļ��ڵ�ָ��
 *	@param struct file *filp �豸�ļ�ָ��
 *	@return	0:�������أ�-1:���󷵻�
 */
int ctest_open(struct inode *inode, struct file *filp)
{
  /* ���豸�ṹ��ָ�븳ֵ���ļ�˽������ָ�� */
	filp->private_data = ctest_devp;
	return 0;
}

/**	@fn	int ctest_open(struct inode *inode, struct file *filp)
 *	@brief	�����ͷ��������Դ
 *	@param struct inode *inode �ļ��ڵ�ָ��
 *	@param struct file *filp �豸�ļ�ָ��
 *	@return	0:�������أ�-1:���󷵻�
 */
int ctest_release(struct inode *inode, struct file *filp)
{
	return 0;
}

/**	@fn	static int ctest_ioctl(struct inode *inodep, struct file *filp, unsigned
	int cmd, unsigned long arg)
 *	@brief	ioctl�ӿ�
 *	@param struct inode *inodep �ļ��ڵ�ָ��
 *	@param struct file *filp �豸�ļ�ָ��
 *	@param unsigned int cmd ����
 *	@param unsigned long arg �������
 *	@return	cmd:�������أ���������:���󷵻�
 */
static int ctest_ioctl(struct inode *inodep, struct file *filp, unsigned
  int cmd, unsigned long arg)
{
	/*����豸�ṹ��ָ��*/
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
 *	@brief	��ѵ�ӿں���
 *	@param struct inode *inode �ļ��ڵ�ָ��
 *	@param struct poll_table *wait �ȴ�����
 *	@return	mask ״̬��ʾ
 */
static unsigned int ctest_poll(struct file *filp, poll_table *wait)
{
	unsigned int mask = 0;
	struct ctest_dev *dev = filp->private_data; /*����豸�ṹ��ָ��*/
  
	dbg_prt_line;
	down(&dev->sem);
  
	poll_wait(filp, &dev->r_wait, wait);
	poll_wait(filp, &dev->w_wait, wait);  
	/*�������ɶ�*/
	if (dev->r_index != dev->w_index)
	{
		mask |= POLLIN | POLLRDNORM; /*��ʾ���ݿɻ��*/
	}
	/* ��������д */
	if (dev->w_index + 1 != dev->r_index)
	{
		mask |= POLLOUT | POLLWRNORM; /*��ʾ���ݿ�д��*/
	}
     
	up(&dev->sem);
	return mask;
}


/**	@fn	static ssize_t ctest_read(struct file *filp, char __user *buf, size_t count,
  loff_t *ppos)
 *	@brief	��ȡ�����ӿ�
 *	@param struct inode *inode �ļ��ڵ�ָ��
 *	@param struct file *filp �ļ�ָ��
 *	@param size_t count ��Ҫ���Ƶ��ַ���
 *	@param loff_t *ppos �ļ�ָ��ƫ����
 *	@return	count ���Ƶ��ֽ���
 */
static ssize_t ctest_read(struct file *filp, char __user *buf, size_t count,
  loff_t *ppos)
{
	int ret, num, capability;
	struct ctest_dev *dev = filp->private_data; /* ����豸�ṹ��ָ�� */
	DECLARE_WAITQUEUE(wait, current); /* ����ȴ����� */

	down(&dev->sem); /* ����ź��� */
	add_wait_queue(&dev->r_wait, &wait); /* ����ȴ�����ͷ */

	/* ����������� */
	if (dev->r_index == dev->w_index)
	{
		dbg_prt_line;
		wake_up_interruptible(&dev->w_wait); /* ����д�ȴ����� */

	
		if (filp->f_flags & O_NONBLOCK)
		{
			ret =  - EAGAIN;
			up(&dev->sem); 
			remove_wait_queue(&dev->w_wait, &wait); /* �Ƴ��ȴ����� */
			return ret;
		} 
		set_current_state(TASK_INTERRUPTIBLE); /* �߳����� */
		up(&dev->sem);

		/* ���������߳� */
		schedule(); 

		#if 0
		/* �������Ϊ�źŻ��� */
		if (signal_pending(current))
		{
			ret = - ERESTARTSYS;
			remove_wait_queue(&dev->w_wait, &wait); /* �Ƴ��ȴ����� */
			set_current_state(TASK_RUNNING);
			return ret;
		}
		#endif
		
		down(&dev->sem);
	}
	dbg_prt_line;
	
	/* ���㸴�Ƴ��� */
	capability = dev->w_index - dev->r_index;
	if (capability < 0)
	{
		capability += CTEST_SIZE;
	}
	num = dev->mem - dev->r_index + CTEST_SIZE;
	num = num < capability ? num : capability;
	count = (count <= (u32)num) ? count : num;

	/* ���� */
	copy_to_user(buf, dev->r_index, count);
	wake_up_interruptible(&dev->w_wait); /* ����д�ȴ����� */
	ret = count;
  
	/* ����ָ�� */
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
	remove_wait_queue(&dev->w_wait, &wait); /* �Ƴ��ȴ����� */
	set_current_state(TASK_RUNNING);
	return ret;
}


/**	static ssize_t ctest_write(struct file *filp, const char __user *buf,
  size_t count, loff_t *ppos)
 *	@brief	д�����ӿ�
 *	@param struct inode *inode �ļ��ڵ�ָ��
 *	@param struct file *filp �ļ�ָ��
 *	@param size_t count ��Ҫ���Ƶ��ַ���
 *	@param loff_t *ppos �ļ�ָ��ƫ����
 *	@return	count ���ظ��Ƶ��ֽ���
 */
static ssize_t ctest_write(struct file *filp, const char __user *buf,
  size_t count, loff_t *ppos)
{
	/* ����豸�ṹ��ָ�� */
	struct ctest_dev *dev = filp->private_data; 
	int ret, num, capability;
	DECLARE_WAITQUEUE(wait, current); 

	down(&dev->sem); 
	add_wait_queue(&dev->w_wait, &wait); /* ����д�ȴ�����ͷ */

	/* ����������� */
	if (dev->w_index + 1 == dev->r_index || CTEST_DATA_SIZE == dev->w_index - dev->r_index)
	{
		dbg_prt_line;
		wake_up_interruptible(&dev->r_wait); /* ���Ѷ��ȴ����� */

		/* ����Ƿ��������� */
		if (filp->f_flags & O_NONBLOCK)
		{
			ret =  - EAGAIN;
			up(&dev->sem); 
			remove_wait_queue(&dev->w_wait, &wait); /* �Ƴ��ȴ����� */
			return ret;
		} 
		set_current_state(TASK_INTERRUPTIBLE); /* �߳����� */
		up(&dev->sem);

		schedule(); /* ���������߳� */
		
		#if 0
		/* ���Ϊ�źŻ��� */
		if (signal_pending(current))
		{
			ret =  - ERESTARTSYS;
			remove_wait_queue(&dev->w_wait, &wait); /* �Ƴ��ȴ����� */
			set_current_state(TASK_RUNNING);
			return ret;
		}
		#endif
		
		down(&dev->sem); 
	}
  
	dbg_prt_line;
	
	/* ���㸴�Ƴ��� */
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
  
	/* ����ָ�� */
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

	/* ���Ѷ��ȴ����� */
	wake_up_interruptible(&dev->r_wait);
	ret = count;
	//dev_dbg("w_index = %d\n",dev->w_index - dev->mem);
	up(&dev->sem); //�ͷ��ź���
	remove_wait_queue(&dev->w_wait, &wait); /* �Ƴ��ȴ����� */
	set_current_state(TASK_RUNNING); /* �����߳����� */
	return ret;
}



/**	static ssize_t ctest_proc_read(char *page, char **start, off_t off, int count,
  int*eof, void *data)
 *	@brief	�������ӿ�
 *	@param char *page ҳָ��
 *	@param char **start ��ʵ��ַָ��
 *	@param off_t off ƫ����
 *	@param int count �����ֽ���
 *	@return	len ʵ�ʸ��Ƶ��ֽ��� 
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
 *	@brief	дproc�����ӿ�
 *	@param char *page ҳָ��
 *	@param char **start ��ʵ��ַָ��
 *	@param off_t off ƫ����
 *	@param int count �����ֽ���
 *	@return	len ʵ�ʸ��Ƶ��ֽ��� 
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
 *	@brief	proc�ӿڳ�ʼ��
 *	@return	len ʵ�ʸ��Ƶ��ֽ��� 
 */
static int ctest_proc_init(void)
{
	dbg_prt_line;
	ctest_proc_entry = create_proc_entry("ctest_proc", 0666, NULL); //����/proc
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
 *	@brief	proc�ӿ�ж��
 *	@return	len ʵ�ʸ��Ƶ��ֽ��� 
 */
static void ctest_proc_exit(void)
{
	dbg_prt_line;
	remove_proc_entry("ctest_proc", &proc_root); //����/proc
}






/**	static const struct file_operations ctest_fops
 *	@brief	�ļ������ṹ��
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
 *	@brief	дproc�����ӿ�
 *	@param struct ctest_dev *dev �豸�ṹ��ָ��
 *	@param int index ���豸��
 *	@return	��
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
 *	@brief	�豸ģ�����
 *	@return	0 ��������ֵ �쳣
 */
int ctest_init(void)
{
	int ret;
	dev_t devno = MKDEV(ctest_major, 0);
	dbg_prt_line;

	/* �����豸��*/
	if (ctest_major)
        ret = register_chrdev_region(devno, 1, "ctest");
	else  /* ��̬�����豸�� */
	{
		ret = alloc_chrdev_region(&devno, 0, 1, "ctest");
		ctest_major = MAJOR(devno);
	}
	if (ret < 0)
	{
		return ret;
	}
	/* ��̬�����豸�ṹ����ڴ� */
	ctest_devp = kmalloc(sizeof(struct ctest_dev), GFP_KERNEL);
	if (!ctest_devp)    /*����ʧ��*/
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

    init_MUTEX(&ctest_devp->sem);   /* ��ʼ���ź��� */
    init_waitqueue_head(&ctest_devp->r_wait); /* ��ʼ�����ȴ�����ͷ */
    init_waitqueue_head(&ctest_devp->w_wait); /* ��ʼ��д�ȴ�����ͷ */
	ctest_devp->w_index = ctest_devp->mem;
	ctest_devp->r_index = ctest_devp->mem;
  
	ctest_setup_cdev(ctest_devp, 0);
  
	return 0;
}
/**	static int ctest_init(void)
 *	@brief	ģ��ж�غ���
 *	@return	0 ��������ֵ �쳣
 */
void ctest_exit(void)
{
	dbg_prt_line;

	ctest_proc_exit();
	free_pages((unsigned long) ctest_devp->mem,10);
	cdev_del(&(ctest_devp->cdev));   /* ע��cdev */
	kfree(ctest_devp);     /* �ͷ��豸�ṹ���ڴ� */
	unregister_chrdev_region(MKDEV(ctest_major, 0), 1); /* �ͷ��豸�� */
  
	dev_info("remove ctest successfully!\n");
}

MODULE_AUTHOR("Qin Huatao");
MODULE_LICENSE("Dual BSD/GPL");

module_param(ctest_major, int, S_IRUGO);

module_init(ctest_init);
module_exit(ctest_exit);
