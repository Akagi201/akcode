      
/**@file global.h
 * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief  ͳһ������������������ͷ�ļ������Լ���ӡ��
 * 
 * @author   liuboyf1
 * @date     2012-8-30
 * @version  V1.0.0
 * 
 * @note ///Description here 
 * @note History:        
 * @note     <author>   <time>    <version >   <desc>
 * @note  
 * @warning  
 */

#ifndef CHARDEVICE_DRV_GLOBAL_H_
#define CHARDEVICE_DRV_GLOBAL_H_

#if defined (__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include <linux/init.h>
#include <linux/sched.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/vmalloc.h>
#include <linux/gfp.h> 
#include <linux/poll.h> 
#include <asm/uaccess.h> 

// ����NULLָ��
#ifndef NULL
#if defined (__cplusplus) || defined(c_plusplus)
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

// �Զ�����������
#ifndef TYPE_UINT8
#define TYPE_UINT8
typedef unsigned char UINT8;
#endif

#ifndef TYPE_UINT16
#define TYPE_UINT16
typedef unsigned short UINT16;
#endif

#ifndef TYPE_UINT32
#define TYPE_UINT32
typedef unsigned int UINT32;
#endif

#ifndef TYPE_INT8
#define TYPE_INT8
typedef char INT8;
#endif

#ifndef TYPE_INT16
#define TYPE_INT16
typedef short INT16;
#endif

#ifndef TYPE_INT32
#define TYPE_INT32
typedef int INT32;
#endif

#ifndef TYPE_FLOAT
#define TYPE_FLOAT
typedef float FLOAT;
#endif


// ��ӡ��
#if (defined(DEBUG_PRINT) || defined(DEBUG_WITH_LINE))

    // ��ӡ�ļ���������������ӡ��Ϣ�����е���Ϣ
    #ifdef DEBUG_WITH_LINE
        #define DBG_PRINT_LINE(void) do{printk(KERN_ALERT "[%s] [%s]:[%i]", __FILE__, __func__, __LINE__);}while(0)
    #else
        #define DBG_PRINT_LINE(void) do{} while(0)
    #endif

    #define DBG_PRINT(fmt, args...)             \
    do                                          \
    {                                           \
        DBG_PRINT_LINE(void);                   \
        printk(KERN_ALERT  fmt "\n", ## args);  \
    }while(0)

    #define ERR_PRINT(fmt, args...) do{printk(KERN_ERR fmt, ## args);}while (0)
#else 
    #define DBG_PRINT(fmt, args...) do{} while(0)
    #define ERR_PRINT(fmt, args...) do{} while(0)
#endif  


#endif // end of CHARDEVICE_DRV_GLOBAL_H_

