      
/** @file globalmem.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    虚拟字符驱动的例子
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

#define GLOBALMEM_SIZE 0x1000  /* 全局内存最大4k字节 */
#define MEM_CLEAR 0x1          /* 清0全局内存 , ioctl命令码*/
#define GLOBALMEM_MAJOR 250    /* 预设的globalmem的主设备号 */

static int globalmem_major = GLOBALMEM_MAJOR;

/* globalmem设备结构体,用面向对象的思想对cdev重新封装,以便于我们的操作 */
struct globalmem_dev
{
    struct cdev cdev;  /* cdev结构体 */
    unsigned char mem[GLOBALMEM_SIZE];  /* 全局内存 */
};

struct globalmem_dev *globalmem_devp; /* 声明一个全局的设备结构体指针 */

/* 文件打开函数,用来注册到file_operations结构中open */
int globalmem_open(struct inode *inode, struct file *filp)
{
    /* 将设备结构体指针赋值给文件私有数据指针 */
    /* 当有多个同类设备时, 用私有变量访问很有必要也很方便 */
    filp->private_data = globalmem_devp;
    return 0;
}

/* 文件释放函数, 用来注册到file_operations结构中release */
int globalmem_release(struct inode *inode, struct file *filp)
{
    return 0;
}

/* ioctl设备控制函数, 用来注册到file_operations结构中toctl */
static int globalmem_ioctl(struct inode *inodep, struct file *filp, unsigned int cmd, unsigned long arg)
{
    struct globalmem_dev *dev = filp->private_data; /* 获得设备结构体指针 */

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

/* 读函数, 用来注册到file_operations 结构中的read ，返回读取到的字节数*/
static ssize_t globalmem_read(struct file *filp, char __user *buf, size_t size, loff_t *ppos)
{
    unsigned long p = *ppos;   /* 获取到当前全局内存的偏移量 */
    unsigned int count = size; /* 获取到要读取全局内存的大小 */
    int ret = 0;               /* 用来记录返回值 */
    struct globalmem_dev *dev = filp->private_data; /* 获得设备结构体指针 */

    /* 分析和获取有效的读长度, 就是看给的要读取的长度是否合法 */
    if (p >= GLOBALMEM_SIZE)
    {
        return 0;
    }
    if (count > GLOBALMEM_SIZE - p) /* 如果要读取的量比剩余的还多，只给它可读到的量 */
    {
        count = GLOBALMEM_SIZE - p;
    }
    /*
     * 一切就绪后就开始往用户空间读了,这里的读指的是用户空间的读，就是说
     * 从内核拷贝到用户空间，因为内核是万能的，有着所有的权限，主动权在于它，
     * 并不是说用户想读就可以读的，因为该空间是在内核状态下分配出来的。
     */

    /* 内核空间 -> 用户空间 */
    /* 从mem的偏移量p处拷贝count个数据到buf所指区 */
    if (copy_to_user(buf, (void *)(dev->mem + p), count))
    {
        ret = -EFAULT;
    }
    else
    { /* 拷贝成功, 返回重新拷贝的数据量并重新计算偏移量 */
        *ppos += count;
        ret = count;

        printk(KERN_EMERG "read %u byte(s) from %lu\n", count, p);
    }
    return ret;
}

/* 写函数, 用来注册到file_operations结构中的write */
static ssize_t globalmem_write(struct file *filp, const char __user *buf, size_t size, loff_t *ppos)
{
    unsigned long p = *ppos;    /* 获取到当前全局内存的偏移量 */
    unsigned int count = size;  /* 获取到要读取数据的大小 */
    int ret = 0;                /* 用来记录返回值 */
    struct globalmem_dev *dev = filp->private_data; /* 获得设备结构体指针 */

    /* 分析和获取有效的写长度, 就是看给的要写的长度是否合法 */
    if (p >= GLOBALMEM_SIZE)
    {
        return 0;
    }
    if (count > GLOBALMEM_SIZE - p) /* 如果要读取的量比剩余的还多，只给它可读到的量 */
    {
        count = GLOBALMEM_SIZE -p;
    }
    /*
     * 一切就绪后就开始放共享空间里写了，这里的写指的就是说从用户空间拷贝到内核
     * ，因为内核是万能的有着所有的权限，主动权在于它，并不是说用户想写就可以写
     * 的，因为该空间是在内核状态下分配出来的
     */


    /* 用户空间 -> 内核空间 */
    /* 往mem的偏移量p处拷贝count个数据(从buf所指区) */
    if (copy_from_user(dev->mem + p, buf, count))
    {
        ret = -EFAULT;
    }
    else
    { /* 拷贝成功，返回重新拷贝的数据量并重新计算偏移量 */
        *ppos += count;
        ret = count;
        printk(KERN_EMERG "written %u byte(s) from %lu\n", count, p);
    }
    return ret;
}

/* seek文件定位函数 */
static loff_t globalmem_llseek(struct file *filp, loff_t offset, int orig)
{
    loff_t ret = 0;
    switch (orig)
    {
        case 0: /* 相对文件开始位置调整 */
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
        case 1: /* 相对文件当前位置偏移 */
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

/* 文件操作结构体 */
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

/* 初始化并注册cdev */
static void globalmem_setup_cdev(struct globalmem_dev *dev, int index)
{
    int err;
    /* MKDEV宏来生成设备号主设备号占12位，次设备号占20位 */
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

/***********************模块相关函数************************/
/*
 * 为了便于开发和排错，建议上来先定义这些模块相关的初始化和卸载函数，你完全可以
 * 只做一个空的函数实现，意在每写一个功能的函数就编译(make)一次，这样会很有利于
 * 开发的正确性
 */

/* 设备驱动模块加载函数 */
int globalmem_init(void)
{
    int result; // 返回值
	/* 表示主设备号是globalmem_major, 次设备号是0 */
    dev_t devno = MKDEV(globalmem_major, 0);

    /* 静态申请 */
    if (globalmem_major) /* 当指定了主设备号 */
    {
        result = register_chrdev_region(devno, 1, "globalmem");
    }
    else /* 动态分配 */
    {
		/* &devno通过引用来取得返回的设备号(包括主设备和次设备) */
        result = alloc_chrdev_region(&devno, 0, 1, "globalmem");
        globalmem_major = MAJOR(devno);
    }
    if (result < 0)
    {
        return result;
    }

    /* kmalloc定义在<linux/slab.h> */
    /* 动态申请设备结构体用到的全局共享内存 */
    globalmem_devp = kmalloc(sizeof(struct globalmem_dev), GFP_KERNEL);
    if (!globalmem_devp)  /* 申请失败 */
    {
        result = -ENOMEM;
        goto fail_malloc;
    }

	// 初始化globalmem_devp数据(清空)
    memset(globalmem_devp, 0, sizeof(struct globalmem_dev));

	// 自定义的初始化并注册cdev的函数，参数0是次设备号
    globalmem_setup_cdev(globalmem_devp, 0);
    return 0;

fail_malloc: /* 分配失败后要还原现场，把已经注册的设备给释放掉 */
    unregister_chrdev_region(devno, 1); /* 释放设备号 */
    return result;
}

/* 模块卸载函数 */
void globalmem_exit(void)
{
    cdev_del(&globalmem_devp->cdev);  /* 注销cdev */
    kfree(globalmem_devp); /* 释放设备结构体内存 */
    unregister_chrdev_region(MKDEV(globalmem_major, 0), 1); /* 释放设备号 */
}

MODULE_AUTHOR("liuboyf1, liuboyf1@hikvision.com.cn");
MODULE_LICENSE("Dual BSD/GPL");

module_param(globalmem_major, int, S_IRUGO);

module_init(globalmem_init);
module_exit(globalmem_exit);

