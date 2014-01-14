      
/** @file book.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    演示 module_param 命令行传递参数用法
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

#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");

static char *book_name = "Dissecting Linux Device Driver";
static int num = 4000;

static int book_init(void)
{
    printk(KERN_EMERG " Book name : %s\n", book_name);
    printk(KERN_EMERG " Book num : %d\n", num);
    return 0;
}

static void book_exit(void)
{
    printk(KERN_EMERG " Book module exit\n");
}

module_init(book_init);
module_exit(book_exit);
module_param(num, int, S_IRUGO);
module_param(book_name, charp, S_IRUGO);

MODULE_AUTHOR("liuboyf1, liuboyf1@hikvision.com.cn");
MODULE_DESCRIPTION("A simple Module for testing module params");
MODULE_VERSION("V1.0");





