      
/** @file export_symbol.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    导出函数演示
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


#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");

int add_integer(int a, int b)
{
    return a + b;
}

int sub_integer(int a, int b)
{
    return a - b;
}

EXPORT_SYMBOL(add_integer);
EXPORT_SYMBOL(sub_integer);

