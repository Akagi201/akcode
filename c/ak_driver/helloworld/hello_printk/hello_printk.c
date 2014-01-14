      
/** @file hello_printk.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    Çý¶¯hello world
  * 
  * @author   liuboyf1
  * @date     2012-9-20
  * @version  V1.0.0
  * 
  * @note ///Description here 
  * @note History:    
  * @note     <author>   <time>    <version >   <desc>
  * @note  
  * @warning  
  */

#include <linux/init.h> /* Needed for the macros */
#include <linux/module.h> /* Needed for all modules */
#include <linux/kernel.h> /* Needed for KERN_EMERG */

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("liuboyf1");
MODULE_DESCRIPTION("\"Hello, world!\" minimal module");
MODULE_VERSION("printk");

static int __initdata hello_data = 3;

static int __init hello_init(void)
{
    printk(KERN_EMERG "Hello, world! %d\n", hello_data);
    return 0;
    //return -1;
}

static void __exit hello_exit(void)
{
    printk(KERN_EMERG "Goodbye, world!\n");
}

module_init(hello_init);
module_exit(hello_exit);

