      
/** @file ak_typedefs.h
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    定义一些类型定义,保证设备上的应用层代码可以在我这里测试用
  * 
  * @author   liuboyf1
  * @date     2013-1-25
  * @version  V1.0.0
  * 
  * @note ///Description here 
  * @note History:    
  * @note     <author>   <time>    <version >   <desc>
  * @note  
  * @warning  
  */

#ifndef AK_TYPEDEFS_H_
#define AK_TYPEDEFS_H_ (1)

#ifndef OK
#define OK (0)
#endif

#ifndef TRUE
#define TRUE (1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif

#ifndef TYPE_BOOL
#define TYPE_BOOL
typedef int BOOL;
#endif

#ifndef  TYPE_BYTE
#define  TYPE_BYTE
typedef unsigned char 	BYTE;
#endif

#ifndef  TYPE_UCHAR
#define  TYPE_UCHAR
typedef unsigned char	UCHAR;
#endif

#ifndef  TYPE_UINT8
#define  TYPE_UINT8
typedef unsigned char 	UINT8;
#endif

#ifndef  TYPE_PUINT8
#define  TYPE_PUINT8
typedef unsigned char*	 PUINT8;
#endif

#ifndef  TYPE_UINT16
#define  TYPE_UINT16
typedef unsigned short UINT16;
#endif

#ifndef  TYPE_UINT32
#define  TYPE_UINT32
typedef unsigned int 	UINT32;
#endif

#ifndef  TYPE_UINT64
#define  TYPE_UINT64
typedef unsigned long long UINT64;
#endif

#ifndef  TYPE_ULONG
#define  TYPE_ULONG
typedef unsigned long 	ULONG;
#endif

#ifndef  TYPE_UINT
#define  TYPE_UINT
typedef unsigned int	UINT;
#endif

#ifndef  TYPE_PUINT
#define  TYPE_PUINT
typedef unsigned int*	PUINT;
#endif

#ifndef  TYPE_INT
#define  TYPE_INT
typedef int INT;
#endif

#ifndef  TYPE_INT8
#define  TYPE_INT8
typedef signed char 	INT8;
#endif

#ifndef  TYPE_INT16
#define  TYPE_INT16
typedef short	INT16;
#endif

#ifndef  TYPE_INT32
#define  TYPE_INT32
typedef int	INT32;
#endif

#ifndef TYPE_INT64
#define TYPE_INT64
typedef long long 	INT64;
#endif

#ifndef  TYPE_PINT
#define  TYPE_PINT
typedef int*	PINT;
#endif

#ifndef  TYPE_PVOID
#define  TYPE_PVOID
typedef void*	PVOID;
#endif

#ifndef  TYPE_VOID
#define  TYPE_VOID
typedef void	VOID;
#endif

#ifndef  TYPE_UINTV
#define  TYPE_UINTV
typedef volatile UINT	UINTV;
#endif

#ifndef  TYPE_PUINTV
#define  TYPE_PUINTV
typedef volatile UINT *	PUINTV;
#endif

#ifndef TYPE_NULL
#define NULL ((void *) 0)
#endif

#endif // AK_TYPEDEFS_H_

