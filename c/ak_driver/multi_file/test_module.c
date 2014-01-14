      
/** @file test_module.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief 1.    ������.cԴ�ļ���ko�������ļ���д������̫��Ľ̳̣����cԴ�ļ�������������������ģ�
  * @brief 2.    ��д���������ں�ģ�飨���˳��Ҳ�����£���Ȼò��û���õ���
  * @brief 3.    insmod,rmmod,mknod,modprobe,dmesg������ʹ�ã�dmesg�Ƚ����ã���Щ�˲�֪������������˵Ҫ�ô�����ʾ��Ϣ����ʵdmesg��������㣬-cѡ�������Ϣ��
  * @brief 4.    �޷������ں˵�ԭ�򡣣����Ҳ�ǹؼ����ܶ�ʱ����������Ƚϼ򵥣���������ں�Դ�룬�����ܻ�����һϵ�д���,���һ�Ѵ�����������һ���˵�������
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

