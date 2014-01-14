      
/** @file mycdev.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    һ����򵥵��ַ��豸����
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

#include <linux/init.h>   /* ��ʼ��ͷ�ļ� */
#include <linux/module.h> /* �������ͷ�ļ���֧�ֶ�̬��Ӻ�ж��ģ�顣hello world ����Ҫ��һ���ļ��Ϳ����� */
#include <linux/types.h> /* ��һЩ�������͵Ķ��壬����dev_t, off_t, pid_t, ��ʵ��Щ���ʹ󲿷ֶ���unsigned int���ͣ�Ҳ��unsigned long */
#include <linux/fs.h>  /* ������struct inode �Ķ��壬MINOR, MAJOR��ͷ�ļ� */
#include <asm/uaccess.h>
#include <linux/cdev.h> /* ������cdev�ṹ����غ����Ķ��� */

// �ҵĴ�ӡ�궨��
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
// 231��û�б�ʹ�õ����豸�ţ�����cat /proc/devices���鿴�豸��ʹ�����
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
 * ����ssize_t:
 * ��<linux/types.h>��: typedef __kernel_ssize_t ssize_t;
 * ��������: typedef long __kernel_ssize_t;
 */
/*
 * ����size_t:
 * ��<linux/types.h>��: typedef __kernel_size_t size_t;
 * �������룺 typedef unsigned long __kernel_size_t;
 */
/*
 * ����loff_t:
 * ��<linux/types.h>��: typedef __kernel_loff_t loff_t;
 * �������룺typedef long long __kernel_loff_t;
 */
/*
 * ����__user,��ʱû�ҵ���ϸ���ϣ�Ӧ���Ǳ�ʶ*buf�����û��ռ�
 */
/*
 * ssize_t (*read) (struct file *filp, char __user *buffer, size_t size, loff_t
 * *p);
 * flip:���ж�ȡ��Ϣ��Ŀ���ļ�
 * buffer:��Ӧ��ֹ��Ϣ�Ļ�����(���û��ռ��ڴ��ַ)
 * size: Ҫ��ȡ����Ϣ����
 * p : ����λ��������ļ���ͷ��ƫ�ƣ��ڶ�ȡ��Ϣ�󣬸�ָ��һ�㶼���ƶ�
 * �ƶ���ֵ����Ҫ��ȡ����Ϣ�ĳ���
 * ��������������豸�л�ȡ���ݣ������λ�õ�һ����ָ�뵼��readϵͳ������
 * -EINVAL("Invalid argument")ʧ�ܡ�
 *  һ���Ǹ�����ֵ�����˳ɹ���ȡ���ֽ���
 */


static ssize_t mycdev_read(struct file *fp, char __user *buf, size_t size, loff_t *pos)
{
    unsigned long p = *pos;
    unsigned int count = size;
    //int i;
    char kernel_buf[MYCDEV_SIZE]= "This is mycdev!";
    if (p >= MYCDEV_SIZE) /* ��ȡ��ƫ�ƾ��볬��mycdev�����С */
    {
        return -1;
    }
    
    if (count > MYCDEV_SIZE) /* Ҫ��ȡ���ַ���������ʣ�ַ�������Ҫ����count */
    {
        count = MYCDEV_SIZE - p;
    }

    /*
     * copy_to_user(void __user *to, const void *from, unsigned long n)
     * �书���ǽ��ں˿ռ�����ݸ��Ƶ��û��ռ�
     * to : Ŀ���ַ�������ַ���û��ռ�ĵ�ַ��
     * from : Դ��ַ�������ַ���ں˿ռ�ĵ�ַ��
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
 * filling the mycdev's file operation interface in the struct file_operations  * ����������+����һ���ṹ����� mycdev_fops, file_operations�ṹ��Ķ�����ͷ
 * �ļ�<linux/fs.h>�У�����û��ÿһ�ʵ��
 */

static const struct file_operations mycdev_fops = 
{
    .owner = THIS_MODULE, /* ָ��ӵ������ṹ���ģ���ָ�� */
    .read = mycdev_read,
    .write = mycdev_write,
    .open = mycdev_open,
    .release = mycdev_release,
    /* �����һ�����豸���û�����ִ��close()ϵͳ����ʱ���ں˽�������������release()���� */
};

/* module loading function */
/*
 * __initλ�� : include/asm-i386/init.h
 * ����: #define __init __attribute__ ((__section__ (".text.init")))
 * ע��: �����ʶ���ͺ�����������һ�𣬱���gcc�������ڱ����ʱ����Ҫ���������
 * ����.text.init section�У������section���ں���ɳ�ʼ��֮�󣬻ᱻ�ͷŵ���
 * �� __init���εĺ������ǳ�������ʾ���õģ����Ǳ������ض���(���룬����)���е�
 * ���ڳ�ʼ����ʱ����á�
 * ģ����ں�����ģ���ʼ����������ģ���ʼ��ʱ���У�����ע��ģ�����ṩ�ģ�����
 * ��Ӧ�ó�����ʵ��¹��ܡ�
 * __init ��ʾ�ú���ֻ�ڳ�ʼ���ڼ�ʹ�ã�ģ�鱻װ�غ�ģ���ʼ����������ģ��װ
 * �������������������ͷŸú���ʹ�õ��ڴ�
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
/*  ����int register_chrdev(unsigned int major, const char *name,
 *  struct file_operations *fops); ���У�major��Ϊ�豸����������ϵͳ�����
 *  ���豸�ţ����Ϊ0��ϵͳΪ����������̬�ط���һ�����豸�š� 
 *  name���豸����fops����ǰ����˵�ĶԸ������õ���ڵ��˵�����˺�������0��ʾ��
 *  ��������-EINVAL��ʾ��������豸�ŷǷ���һ����˵�����豸�Ŵ���ϵͳ���������
 *  ���豸�š�����-EBUSY��ʾ����������豸�����ڱ������豸��������ʹ�á� ���
 *  �Ƕ�̬�������豸�ųɹ����˺�������������������豸�š����register_chrdev��
 *  ���ɹ��� �豸��(����name)�ͻ������/proc/devices�ļ�� �ڳɹ�����ϵͳע��
 *  ���豸���������(����register_chrdev()�ɹ���)���Ϳ�����mknod���������豸ӳ��
 *  Ϊһ���ر��ļ��� ��������ʹ������豸��ʱ��ֻҪ�Դ��ر��ļ����в��������� 
 *  ��Linux2.6�汾���棬register_chrdev_region()��register_chrdev()�������汾��
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
 * ģ����ں�������ģ���������,��ģ�鱻�Ƴ�ǰע��ģ��ӿڲ���ϵͳ����������
 * Դ,���纯���ж�̬������ڴ��.
 * .__exit�ĺ�������ں�����__init����,������ģ��ж��,���ģ�鱻ֱ��Ƕ���ں�,��
 * ���ں����ò�����ж��ģ��,��ú�����ֱ������.
 */

static void __exit mycdev_exit(void)
{
    printk(KERN_EMERG "mycdev module is leaving..\n");
    unregister_chrdev(MYCDEV_MAJOR, "liuboyf1_cdev");
   // unregister_chrdev_region(MKDEV(mycdev_major, 0), 1);  /* �ͷ��豸�� */
}


/*
 * ���ڼ���ں�����ע�Ჿ��.
 * ģ��ĳ��ڼ���ں����ֱ�ʹ�ú�module_init()��module_exit().
 * ��ں���ע��:module_init(my_init),����my_initΪģ����ں�����.
 * ���ں���ע��:module_exit(my_exit),����my_exitΪģ����ں�����.
 */

module_init(mycdev_init);
module_exit(mycdev_exit);












