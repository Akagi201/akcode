      
/** @file test_module.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief 1.    编译多个.c源文件的ko（单个文件编写网上有太多的教程，多个c源文件编译是这我这回遇到的）
  * @brief 2.    编写带参数的内核模块（这个顺便也给记下，虽然貌似没有用到）
  * @brief 3.    insmod,rmmod,mknod,modprobe,dmesg等命令使用（dmesg比较有用，有些人不知道用这个命令，就说要用串口显示信息，其实dmesg命令更方便，-c选项清空信息）
  * @brief 4.    无法插入内核的原因。（这个也是关键，很多时候操作起来比较简单，比如编译内核源码，但可能会碰到一系列错误,解决一堆错误最能提升一个人的能力）
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

#include "extern_app.h"

static char *who = "linux ss";
static int many = 1;
module_param(many, int, S_IRUGO);
module_param(who, charp, S_IRUGO);

static int __init hello_init(void)
{
    test();
    static int i = 0;

    for(i = 0; i < many; i++)
        printk(KERN_EMERG "Hello, %s!\n", who);

    return 0;
}

static void __exit hello_exit(void)
{
    printk(KERN_EMERG "Goodbye\n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");

