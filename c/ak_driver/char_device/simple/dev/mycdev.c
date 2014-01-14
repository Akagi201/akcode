      
/** @file mycdev.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    一个最简单的字符设备驱动
  * 
  * @author   liuboyf1
  * @date     2012-9-24
  * @version  V1.0.0
  * 
  * @note ///Description here 
  * @note History:    
  * @note     <author>   <time>    <version >   <desc>
  * @note  
  * @warning  
  */

#include <linux/init.h>   /* 初始化头文件 */
#include <linux/module.h> /* 最基本的头文件，支持动态添加和卸载模块。hello world 驱动要这一个文件就可以了 */
#include <linux/types.h> /* 对一些特殊类型的定义，例如dev_t, off_t, pid_t, 其实这些类型大部分都是unsigned int类型，也有unsigned long */
#include <linux/fs.h>  /* 包含了struct inode 的定义，MINOR, MAJOR的头文件 */
#include <asm/uaccess.h>
#include <linux/cdev.h> /* 包含了cdev结构及相关函数的定义 */

// 我的打印宏定义
#ifdef DEBUG
    #ifdef __KERNEL__
        #define DBG_PRINT_FILE_LINE(void)                                    \
        do                                                                   \
        {                                                                    \
            printk(KERN_EMERG "\n%s:%s():%d:", __FILE__, __func__, __LINE__);\
        }while (0)

        #define DBG_PRINT(fmt, args...)                                      \
        do                                                                   \
        {                                                                    \
            DEB_PRINT_FILE_LINE(void);                                       \
            printk(KERN_EMERG fmt, ##args);                                  \
        }while (0)
        
        #define ERR_PRINT(fmt, args...)                                      \
        do                                                                   \
        {                                                                    \
            printk(KERN_EMERG fmt, ##args);                                  \
        }while (0)
    #else
        #define DBG_PRINT_FILE_LINE(void)                                    \
        do                                                                   \
        {                                                                    \
            fprintf(stderr, "\n%s:%s():%d:", __FILE__, __func__, __LINE__);  \
        }while (0)
        #define DBG_PRINT(fmt, args...)                                      \
        do                                                                   \
        {                                                                    \
            DBG_PRINT_FILE_LINE(void); fprintf(stderr, fmt, ##args);         \
        }while (0)
        #define ERR_PRINT(str)                                               \
        do                                                                   \
        {                                                                    \
            DBG_PRINT_FILE_LINE(void);                                       \
            perror(str);                                                     \
        }while (0)
    #endif
#else
    #define DBG_PRINT_FILE_LINE(void) do{}while (0)
    #define DBG_PRINT(fmt, args...) do{}while (0)
    #define ERR_PRINT(fmt, args...) do{}while (0)
#endif


MODULE_AUTHOR("liuboyf1");
MODULE_LICENSE("Dual BSD/GPL");

//static int mycdev_major = 231; /* the predefined mycdev's major devno */
// 231是没有被使用的主设备号，可用cat /proc/devices来查看设备号使用情况
#define MYCDEV_MAJOR 231
#define MYCDEV_SIZE 100

static int mycdev_open(struct inode *inode, struct file *fp)
{
    return 0;
}

static int mycdev_release(struct inode *inode, struct file *fp)
{
    return 0;
}

/*
 * 关于ssize_t:
 * 在<linux/types.h>中: typedef __kernel_ssize_t ssize_t;
 * 继续深入: typedef long __kernel_ssize_t;
 */
/*
 * 关于size_t:
 * 在<linux/types.h>中: typedef __kernel_size_t size_t;
 * 继续深入： typedef unsigned long __kernel_size_t;
 */
/*
 * 关于loff_t:
 * 在<linux/types.h>中: typedef __kernel_loff_t loff_t;
 * 继续深入：typedef long long __kernel_loff_t;
 */
/*
 * 关于__user,暂时没找到详细资料，应该是标识*buf属于用户空间
 */
/*
 * ssize_t (*read) (struct file *filp, char __user *buffer, size_t size, loff_t
 * *p);
 * flip:进行读取信息的目标文件
 * buffer:对应防止信息的缓冲区(即用户空间内存地址)
 * size: 要读取的信息长度
 * p : 读的位置相对于文件开头的偏移，在读取信息后，该指针一般都会移动
 * 移动的值就是要读取的信息的长度
 * 这个函数用来从设备中获取数据，在这个位置的一个空指针导致read系统调用以
 * -EINVAL("Invalid argument")失败。
 *  一个非负返回值代表了成功读取的字节数
 */


static ssize_t mycdev_read(struct file *fp, char __user *buf, size_t size, loff_t *pos)
{
    unsigned long p = *pos;
    unsigned int count = size;
    //int i;
    char kernel_buf[MYCDEV_SIZE]= "This is mycdev!";
    if (p >= MYCDEV_SIZE) /* 读取的偏移距离超过mycdev本身大小 */
    {
        return -1;
    }
    
    if (count > MYCDEV_SIZE) /* 要读取的字符数大于所剩字符，所以要修正count */
    {
        count = MYCDEV_SIZE - p;
    }

    /*
     * copy_to_user(void __user *to, const void *from, unsigned long n)
     * 其功能是将内核空间的内容复制到用户空间
     * to : 目标地址，这个地址是用户空间的地址；
     * from : 源地址，这个地址是内核空间的地址；
     */
    if (copy_to_user(buf, kernel_buf, count) != 0)
    {
        printk(KERN_EMERG   "read error!\n");
        return -1;
    }
    
#if 0
    for (i = 0; i < count; ++i)
    {
        __put_user(i, buf); /* write 'i' from kernel space to user space's buf */
        ++buf;
    }
#endif

    printk(KERN_EMERG "liuboyf1's reader: %d bytes was read...\n", count);
    return count;
}

static ssize_t mycdev_write(struct file *fp, const char __user *buf, size_t size, loff_t *pos)
{
    return size;
}

/* 
 * filling the mycdev's file operation interface in the struct file_operations  * 这里是声明+定义一个结构体对象 mycdev_fops, file_operations结构体的定义在头
 * 文件<linux/fs.h>中，这里没有每一项都实现
 */

static const struct file_operations mycdev_fops = 
{
    .owner = THIS_MODULE, /* 指向拥有这个结构体的模块的指针 */
    .read = mycdev_read,
    .write = mycdev_write,
    .open = mycdev_open,
    .release = mycdev_release,
    /* 当最后一个打开设备的用户进程执行close()系统调用时，内核将调用驱动程序release()函数 */
};

/* module loading function */
/*
 * __init位置 : include/asm-i386/init.h
 * 定义: #define __init __attribute__ ((__section__ (".text.init")))
 * 注释: 这个标识符和函数声明放在一起，表明gcc编译器在编译的时候需要把这个函数
 * 放在.text.init section中，而这个section在内核完成初始化之后，会被释放掉。
 * 以 __init修饰的函数并非程序中显示调用的，而是被放在特定的(代码，数据)段中的
 * ，在初始化的时候调用。
 * 模块入口函数即模块初始化函数，在模块初始化时运行，负责注册模块所提供的，可以
 * 被应用程序访问的新功能。
 * __init 表示该函数只在初始化期间使用，模块被装载后，模块初始化函数即被模块装
 * 载器丢弃，这样可以释放该函数使用的内存
 */
static int __init mycdev_init(void)
{
    int ret;

    printk(KERN_EMERG "mycdev module is starting...\n");
	DBG_PRINT_FILE_LINE();
    
    /*
    dev_t devno = MKDEV(mycdev_major, 0);

    if (mycdev_major)
    {
        ret = register_chrdev_region(devno, 1, "mycdev");
    }
    else
    {
        ret = alloc_chrdev_region(&devno, 0, 1, "mycdev");
        mycdev_major = MAJOR(devno); 
    }
    */
/*  关于int register_chrdev(unsigned int major, const char *name,
 *  struct file_operations *fops); 其中，major是为设备驱动程序向系统申请的
 *  主设备号，如果为0则系统为此驱动程序动态地分配一个主设备号。 
 *  name是设备名。fops就是前面所说的对各个调用的入口点的说明。此函数返回0表示成
 *  功。返回-EINVAL表示申请的主设备号非法，一般来说是主设备号大于系统所允许的最
 *  大设备号。返回-EBUSY表示所申请的主设备号正在被其它设备驱动程序使用。 如果
 *  是动态分配主设备号成功，此函数将返回所分配的主设备号。如果register_chrdev操
 *  作成功， 设备名(参数name)就会出现在/proc/devices文件里。 在成功的向系统注册
 *  了设备驱动程序后(调用register_chrdev()成功后)，就可以用mknod命令来把设备映射
 *  为一个特别文件， 其它程序使用这个设备的时候，只要对此特别文件进行操作就行了 
 *  在Linux2.6版本里面，register_chrdev_region()是register_chrdev()的升级版本。
 */

    
    ret = register_chrdev(MYCDEV_MAJOR, "mycdev", &mycdev_fops);
    if (ret < 0)
    {
        printk(KERN_EMERG "register failed..\n");
        return ret;
    }
    else
    {
        printk(KERN_EMERG "register success..\n");
    }

    return 0;
}

/* module unloading function */
/*
 * 模块出口函数负责本模块的清理工作,在模块被移除前注销模块接口并向系统返回所有资
 * 源,例如函数中动态分配的内存等.
 * .__exit的含义与入口函数的__init类似,仅用于模块卸载,如果模块被直接嵌入内核,或
 * 者内核配置不允许卸载模块,则该函数被直接抛弃.
 */

static void __exit mycdev_exit(void)
{
    printk(KERN_EMERG "mycdev module is leaving..\n");
    unregister_chrdev(MYCDEV_MAJOR, "liuboyf1_cdev");
   // unregister_chrdev_region(MKDEV(mycdev_major, 0), 1);  /* 释放设备号 */
}


/*
 * 出口及入口函数的注册部分.
 * 模块的出口及入口函数分别使用宏module_init()及module_exit().
 * 入口函数注册:module_init(my_init),其中my_init为模块入口函数名.
 * 出口函数注册:module_exit(my_exit),其中my_exit为模块出口函数名.
 */

module_init(mycdev_init);
module_exit(mycdev_exit);












