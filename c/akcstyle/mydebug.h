
/** @file mydebug.h
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    我的错误捕捉函数
  *
  * @author   liuboyf1
  * @date     2012-10-9
  * @version  V1.0.0
  *
  * @note ///Description here
  * @note History:
  * @note     <author>   <time>    <version >   <desc>
  * @note
  * @warning
  */

// 我的打印宏定义
#ifdef DEBUG
    #ifdef __KERNEL__
        #define DBG_PRINT_FILE_LINE(void)                                    \
        do                                                                   \
        {                                                                    \
            printk(KERN_EMERG "\n%s:%s():%d:\n", __FILE__, __func__, __LINE__);\
        }while (0)

        #define DBG_PRINT(fmt, args...)                                      \
        do                                                                   \
        {                                                                    \
            DBG_PRINT_FILE_LINE(void);                                       \
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
            fprintf(stderr, "\n%s:%s():%d:\n", __FILE__, __func__, __LINE__);  \
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
