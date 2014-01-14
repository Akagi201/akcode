      
/** @file hello_printk.c
  * @brief    Çý¶¯hello world
  * 
  * @author   liuboyf1
  * @date     2012-10-10
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
MODULE_AUTHOR("Akagi201");
MODULE_DESCRIPTION("\"Hello, world!\" minimal module");
MODULE_VERSION("printk");


static int __init hello_init(void)
{
    printk(KERN_EMERG "Hello, world! --Akagi201");
    return 0;
}

static void __exit hello_exit(void)
{
    printk(KERN_EMERG "Goodbye, world! --Akagi201\n");
}

module_init(hello_init);
module_exit(hello_exit);

