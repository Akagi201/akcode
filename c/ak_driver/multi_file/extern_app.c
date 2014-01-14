      
/** @file extern_app.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    testº¯Êý
  * 
  * @author   liuboyf1
  * @date     2012-9-26
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

int test(void)
{
    printk(KERN_EMERG "test here.\n");
    return 0;
}


