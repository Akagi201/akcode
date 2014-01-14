      
/** @file misc.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    
  * 
  * @author   liuboyf1
  * @date     2012-9-29
  * @version  V1.0.0
  * 
  * @note ///Description here 
  * @note History:    
  * @note     <author>   <time>    <version >   <desc>
  * @note  
  * @warning  
  */

#include <linux/init.h>  
#include <linux/module.h>  
#include <linux/fs.h>  
#include <linux/miscdevice.h>  

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("liuboyf1");
MODULE_DESCRIPTION("misc module");
MODULE_VERSION("misc");

int open_state = 0;  

static int misc_open(struct inode *inode, struct file *filp)  
{  
    if (open_state == 0)	
    {  
        open_state = 1;  
        printk("misc open!\n");  
        return 0;  
    }  
    printk("misc has been open!\n");  
    return -1;  
}  

static int misc_release(struct inode *inode, struct file *filp)  
{  
    if (open_state == 1)	
    {  
        open_state = 0;  
        printk("misc release!\n");	
        return 0;  
    }  
    printk("misc has not been open yet!\n");  
    return -1;  
}  

static ssize_t misc_read(struct file *filp, char *buf,  
    size_t count, loff_t *fpos)  
{  
    printk("misc read!\n");  
    return 0;  
}  

static ssize_t misc_write(struct file *filp, const char __user *buf,  
    size_t count, loff_t *fpos)  
{  
    printk("misc write!\n");  
    return 0;  
}  

static int misc_ioctl(struct inode *inode, struct file *filp,	
    unsigned int cmd, unsigned long arg)	
{  
    printk("ioctl is called!\n");  
    printk("cmd:%u arg:%lu\n", cmd, arg);	
    return 0;  
}  

static const struct file_operations fops =   
{  
    .owner	  =   THIS_MODULE,	
    .open 	  =   misc_open,	
    .release  =   misc_release,  
    .write	  =   misc_write,  
    .read 	  =   misc_read,	
    .ioctl	  =   misc_ioctl	
};  

static struct miscdevice dev =	
{  
    .minor  =   MISC_DYNAMIC_MINOR,  
    .fops    =   &fops,  
    .name   =   "misc",  
    .nodename = "misc_node"  
};  

static int setup_misc(void)  
{  

    return misc_register(&dev);  
}  


static int __init misc_init(void)  
{  
    printk("misc init!\n");  
    return setup_misc();  
}  

static void __exit misc_exit(void)	
{  
    printk("misc exit!\n");  
    misc_deregister(&dev);  
}  


module_init(misc_init);  
module_exit(misc_exit); 

