      
/** @file globalmem.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    �����ַ�����������
  * 
  * @author   liuboyf1
  * @date     2012-9-21
  * @version  V1.0.0
  * 
  * @note ///Description here 
  * @note History:    
  * @note     <author>   <time>    <version >   <desc>
  * @note  
  * @warning  
  */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/uaccess.h>

#define GLOBALMEM_SIZE 0x1000  /* ȫ���ڴ����4k�ֽ� */
#define MEM_CLEAR 0x1          /* ��0ȫ���ڴ� , ioctl������*/
#define GLOBALMEM_MAJOR 250    /* Ԥ���globalmem�����豸�� */

static int globalmem_major = GLOBALMEM_MAJOR;

/* globalmem�豸�ṹ��,����������˼���cdev���·�װ,�Ա������ǵĲ��� */
struct globalmem_dev
{
    struct cdev cdev;  /* cdev�ṹ�� */
    unsigned char mem[GLOBALMEM_SIZE];  /* ȫ���ڴ� */
};

struct globalmem_dev *globalmem_devp; /* ����һ��ȫ�ֵ��豸�ṹ��ָ�� */

/* �ļ��򿪺���,����ע�ᵽfile_operations�ṹ��open */
int globalmem_open(struct inode *inode, struct file *filp)
{
    /* ���豸�ṹ��ָ�븳ֵ���ļ�˽������ָ�� */
    /* ���ж��ͬ���豸ʱ, ��˽�б������ʺ��б�ҪҲ�ܷ��� */
    filp->private_data = globalmem_devp;
    return 0;
}

/* �ļ��ͷź���, ����ע�ᵽfile_operations�ṹ��release */
int globalmem_release(struct inode *inode, struct file *filp)
{
    return 0;
}

/* ioctl�豸���ƺ���, ����ע�ᵽfile_operations�ṹ��toctl */
static int globalmem_ioctl(struct inode *inodep, struct file *filp, unsigned int cmd, unsigned long arg)
{
    struct globalmem_dev *dev = filp->private_data; /* ����豸�ṹ��ָ�� */

    switch (cmd)
    {
        case MEM_CLEAR:
            memset(dev->mem, 0, GLOBALMEM_SIZE);
            printk(KERN_EMERG "globalmem is set to zero.\n");
            break;
        default:
            return -EINVAL;
    }
    return 0;
}

/* ������, ����ע�ᵽfile_operations �ṹ�е�read �����ض�ȡ�����ֽ���*/
static ssize_t globalmem_read(struct file *filp, char __user *buf, size_t size, loff_t *ppos)
{
    unsigned long p = *ppos;   /* ��ȡ����ǰȫ���ڴ��ƫ���� */
    unsigned int count = size; /* ��ȡ��Ҫ��ȡȫ���ڴ�Ĵ�С */
    int ret = 0;               /* ������¼����ֵ */
    struct globalmem_dev *dev = filp->private_data; /* ����豸�ṹ��ָ�� */

    /* �����ͻ�ȡ��Ч�Ķ�����, ���ǿ�����Ҫ��ȡ�ĳ����Ƿ�Ϸ� */
    if (p >= GLOBALMEM_SIZE)
    {
        return 0;
    }
    if (count > GLOBALMEM_SIZE - p) /* ���Ҫ��ȡ������ʣ��Ļ��ֻ࣬�����ɶ������� */
    {
        count = GLOBALMEM_SIZE - p;
    }
    /*
     * һ�о�����Ϳ�ʼ���û��ռ����,����Ķ�ָ�����û��ռ�Ķ�������˵
     * ���ں˿������û��ռ䣬��Ϊ�ں������ܵģ��������е�Ȩ�ޣ�����Ȩ��������
     * ������˵�û�����Ϳ��Զ��ģ���Ϊ�ÿռ������ں�״̬�·�������ġ�
     */

    /* �ں˿ռ� -> �û��ռ� */
    /* ��mem��ƫ����p������count�����ݵ�buf��ָ�� */
    if (copy_to_user(buf, (void *)(dev->mem + p), count))
    {
        ret = -EFAULT;
    }
    else
    { /* �����ɹ�, �������¿����������������¼���ƫ���� */
        *ppos += count;
        ret = count;

        printk(KERN_EMERG "read %u byte(s) from %lu\n", count, p);
    }
    return ret;
}

/* д����, ����ע�ᵽfile_operations�ṹ�е�write */
static ssize_t globalmem_write(struct file *filp, const char __user *buf, size_t size, loff_t *ppos)
{
    unsigned long p = *ppos;    /* ��ȡ����ǰȫ���ڴ��ƫ���� */
    unsigned int count = size;  /* ��ȡ��Ҫ��ȡ���ݵĴ�С */
    int ret = 0;                /* ������¼����ֵ */
    struct globalmem_dev *dev = filp->private_data; /* ����豸�ṹ��ָ�� */

    /* �����ͻ�ȡ��Ч��д����, ���ǿ�����Ҫд�ĳ����Ƿ�Ϸ� */
    if (p >= GLOBALMEM_SIZE)
    {
        return 0;
    }
    if (count > GLOBALMEM_SIZE - p) /* ���Ҫ��ȡ������ʣ��Ļ��ֻ࣬�����ɶ������� */
    {
        count = GLOBALMEM_SIZE -p;
    }
    /*
     * һ�о�����Ϳ�ʼ�Ź���ռ���д�ˣ������дָ�ľ���˵���û��ռ俽�����ں�
     * ����Ϊ�ں������ܵ��������е�Ȩ�ޣ�����Ȩ��������������˵�û���д�Ϳ���д
     * �ģ���Ϊ�ÿռ������ں�״̬�·��������
     */


    /* �û��ռ� -> �ں˿ռ� */
    /* ��mem��ƫ����p������count������(��buf��ָ��) */
    if (copy_from_user(dev->mem + p, buf, count))
    {
        ret = -EFAULT;
    }
    else
    { /* �����ɹ����������¿����������������¼���ƫ���� */
        *ppos += count;
        ret = count;
        printk(KERN_EMERG "written %u byte(s) from %lu\n", count, p);
    }
    return ret;
}

/* seek�ļ���λ���� */
static loff_t globalmem_llseek(struct file *filp, loff_t offset, int orig)
{
    loff_t ret = 0;
    switch (orig)
    {
        case 0: /* ����ļ���ʼλ�õ��� */
            if (offset < 0)
            {
                ret = -EINVAL;
                break;
            }
            if ((unsigned int)offset > GLOBALMEM_SIZE)
            {
                ret = -EINVAL;
                break;
            }
            filp ->f_pos = (unsigned int)offset;
            ret = filp -> f_pos;
            break;
        case 1: /* ����ļ���ǰλ��ƫ�� */
            if ((filp->f_pos + offset) > GLOBALMEM_SIZE)
            {
                ret = -EINVAL;
                break;
            }
            if ((filp->f_pos + offset) < 0)
            {
                ret = -EINVAL;
                break;
            }
            filp->f_pos += offset;
            ret = filp->f_pos;
            break;
        default:
            ret = -EINVAL;
            break;
    }
    return ret;
}

/* �ļ������ṹ�� */
static const struct file_operations globalmem_fops =
{
    .owner = THIS_MODULE,
    .llseek = globalmem_llseek,
    .read = globalmem_read,
    .write = globalmem_write,
    .ioctl = globalmem_ioctl,
    .open = globalmem_open,
    .release = globalmem_release,
};

/* ��ʼ����ע��cdev */
static void globalmem_setup_cdev(struct globalmem_dev *dev, int index)
{
    int err;
    /* MKDEV���������豸�����豸��ռ12λ�����豸��ռ20λ */
    int devno = MKDEV(globalmem_major, index);

    cdev_init(&dev->cdev, &globalmem_fops);
    dev->cdev.owner = THIS_MODULE;
    dev->cdev.ops = &globalmem_fops;
    err = cdev_add(&dev->cdev, devno, 1);
    if (err)
    {
        printk(KERN_EMERG "Error %d adding globalmem %d\n", err, index);
    }
}

/***********************ģ����غ���************************/
/*
 * Ϊ�˱��ڿ������Ŵ����������ȶ�����Щģ����صĳ�ʼ����ж�غ���������ȫ����
 * ֻ��һ���յĺ���ʵ�֣�����ÿдһ�����ܵĺ����ͱ���(make)һ�Σ��������������
 * ��������ȷ��
 */

/* �豸����ģ����غ��� */
int globalmem_init(void)
{
    int result; // ����ֵ
	/* ��ʾ���豸����globalmem_major, ���豸����0 */
    dev_t devno = MKDEV(globalmem_major, 0);

    /* ��̬���� */
    if (globalmem_major) /* ��ָ�������豸�� */
    {
        result = register_chrdev_region(devno, 1, "globalmem");
    }
    else /* ��̬���� */
    {
		/* &devnoͨ��������ȡ�÷��ص��豸��(�������豸�ʹ��豸) */
        result = alloc_chrdev_region(&devno, 0, 1, "globalmem");
        globalmem_major = MAJOR(devno);
    }
    if (result < 0)
    {
        return result;
    }

    /* kmalloc������<linux/slab.h> */
    /* ��̬�����豸�ṹ���õ���ȫ�ֹ����ڴ� */
    globalmem_devp = kmalloc(sizeof(struct globalmem_dev), GFP_KERNEL);
    if (!globalmem_devp)  /* ����ʧ�� */
    {
        result = -ENOMEM;
        goto fail_malloc;
    }

	// ��ʼ��globalmem_devp����(���)
    memset(globalmem_devp, 0, sizeof(struct globalmem_dev));

	// �Զ���ĳ�ʼ����ע��cdev�ĺ���������0�Ǵ��豸��
    globalmem_setup_cdev(globalmem_devp, 0);
    return 0;

fail_malloc: /* ����ʧ�ܺ�Ҫ��ԭ�ֳ������Ѿ�ע����豸���ͷŵ� */
    unregister_chrdev_region(devno, 1); /* �ͷ��豸�� */
    return result;
}

/* ģ��ж�غ��� */
void globalmem_exit(void)
{
    cdev_del(&globalmem_devp->cdev);  /* ע��cdev */
    kfree(globalmem_devp); /* �ͷ��豸�ṹ���ڴ� */
    unregister_chrdev_region(MKDEV(globalmem_major, 0), 1); /* �ͷ��豸�� */
}

MODULE_AUTHOR("liuboyf1, liuboyf1@hikvision.com.cn");
MODULE_LICENSE("Dual BSD/GPL");

module_param(globalmem_major, int, S_IRUGO);

module_init(globalmem_init);
module_exit(globalmem_exit);

