      
/** @file test.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    
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

#include <unistd.h> // read() and write()
#include <stdio.h>
#include <string.h> // strlen()

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// 我的打印宏定义
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


#define CYCLE_CTRL 1000
#define INPUT_CHARS "this is a test"
#define BUF_LEN 14000

int main()
{
	DBG_PRINT_FILE_LINE();
	int i = 0;
	int i_ret = 0;
	int fd = 0;
	int all_count = 0;
	char buf[BUF_LEN + 10] = {0};
		
	fd = open("/dev/scull0", O_RDWR);
	if(-1 == fd)
	{
		ERR_PRINT("open() error! \n");
	}	
	for (i = 0; i < CYCLE_CTRL; i++)
	{
		i_ret = write(fd, INPUT_CHARS, strlen(INPUT_CHARS));
		if(-1 == i_ret)
		{
			ERR_PRINT("write() error 1!\n");
		}
		DBG_PRINT("have write bytes :%d \n", i_ret);
		all_count += i_ret;
	}
	i_ret = write(fd, "\n", strlen("\n"));
	if(-1 == i_ret)
	{
		ERR_PRINT("write() error 1!\n");
	}
	DBG_PRINT("have write all bytes :%d\n ", all_count);
	close(fd);
	
	fd = open("/dev/scull0", O_RDWR);
	if(-1 == fd)
	{
		ERR_PRINT("open() error! \n");
	}
	while(all_count > 0)
	{	
		i_ret = read(fd, buf, BUF_LEN);
		if(-1 == i_ret)
		{
			ERR_PRINT("write() error 2!\n");
		}
		DBG_PRINT("have read bytes :%d \n content :%s\n", i_ret, buf);
		all_count -= i_ret;
	}
	
	return 0;	
}
