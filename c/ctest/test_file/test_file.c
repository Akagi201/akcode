      
/** @file test_file.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    Linux 文件操作用户空间编程（使用系统调用）
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


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define LENGTH 100

int main()
{
    int fd, len;
    char str[LENGTH];

    fd = open("hello.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR); /* 创建并打开文件 */
    if (fd)
    {
        write(fd, "Hello World", strlen("Hello World")); /*写入字符串 */
        close(fd);
    }

    fd = open("hello.txt", O_RDWR);
    len = read(fd, str, LENGTH); /* 读取文件内容 */
    str[len] = '\0';
    printf("%s\n", str);
    close(fd);
    return  0;
}



