      
/** @file update_file.h
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    
  * 
  * @author   liuboyf1
  * @date     2012-9-18
  * @version  V1.0.0
  * 
  * @note ///Description here 
  * @note History:    
  * @note     <author>   <time>    <version >   <desc>
  * @note  
  * @warning  
  */

#ifndef CLIENT_UPDATE_FILE_H_
#define CLIENT_UPDATE_FILE_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <malloc.h>
#include <fcntl.h>
#include <assert.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>
#include <signal.h>

#if defined (__cplusplus) || defined(c_plusplus)
extern "C" {
#endif
// 定义NULL指针
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

// 自定义数据类型
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

#if defined (__cplusplus) || defined(c_plusplus)
}
#endif


#define FILE_NAME_MAX_LEN 100
#define PATH_NAME_MAX_LEN 200
#define BUFFER_SIZE       8*1024
#define LISTEN_MAX           10

extern unsigned short g_server_port;

/** @brief    整个升级包的报文头
  */
typedef struct
{
    unsigned int magic_number;      ///< 魔术字，0x484b5753
    unsigned int header_checksum;   ///< 报文头校验和
    unsigned int header_length;     ///< 报文头长度
    unsigned int file_nums;         ///< 包含的文件个数
    unsigned int language;          ///< 升级包语言，1-英文，2-中文
    unsigned int device_class;      ///< 1 –DS9000 DVR, 2 - IPC
    unsigned int oem_code;          ///< 1 –hikvision
    char            res[36];            ///< 保留字段
}FIRMWARE_HEADER;

/** @brief    每个升级文件的报文头
  */
typedef struct
{
    char file_name[32]; ///< 文件名
    int  start_offset;  ///< 文件起始位置(在升级包中的偏移量)
    int  file_len;      ///< 文件长度
    int  checksum;      ///< 升级文件的校验和，不包含该报文头
}UPGRADE_FILE_HEADER;
 
/** @brief    文件数据包
  *           该结构对文件数据的进行封装，接收端通过该结构对数据进行解析
  */
typedef struct
{
    char file_data_flag;   ///<标志数据包的类型 F<文件数据包> / E<最后一个文件数据包> 
    char reserved[3];      ///<预留
    int file_id;           ///<文件ID号
    int packet_id;         ///<文件数据包的ID号
    int data_len;          ///<文件数据长度
    char buf[BUFFER_SIZE]; ///<文件数据存放缓冲区
}file_packet;

/** @brief    文件数据包的应答数据包
  *           客户端每接收到一个服务器的文件数据包时候的给一个回复
  */
typedef struct
{
    char file_reply_flag;  ///<标志该数据包的类型 Y<文件数据包接收成功>/N <文件数据包接收失败>
    char reserved[3];      ///<预留
    int file_id;           ///<文件ID号
    int packet_id;         ///<表示接收到的文件数据包的ID号
}file_reply_packet;

/** @brief    升级请求数据包
  *           升级请求数据包信息封装结构，发送数据包得到请求
  */
typedef struct
{
    char update_request_flag;               ///<标志该数据包的类型 U<升级请求数据包> 
    char resume_transfer_flag;              ///<断点续传标志 G <续传>/ T <从头开始传>
    char reserved1[2];                      ///<预留
    int resume_transfer_offset;             ///<断点续传偏移量<用已接收到的文件数据的字节数表示>
    time_t mtime;                           ///<记录升级文件的时间属性
    char file_name[FILE_NAME_MAX_LEN + 1];  ///<文件名存储缓冲区
    char reserved2[3];                      ///<预留
}update_request_packet;

/** @brief    升级请求的回复数据包
  *           服务器回复客户端升级请求，确认可否升级
  */
typedef struct
{
    char update_reply_flag;                 ///<标志该数据包的类型 Y<可以升级，不可续传> / N<不可以升级> / G<续传升级> 
    char reserved1[3];                      ///<预留
    time_t mtime;                           ///<记录升级文件的时间属性
    long file_total_size;                   ///<要升级文件的大小
    char file_name[FILE_NAME_MAX_LEN + 1];  ///<文件名存储缓冲区
    char reserved2[3];                      ///<预留
}update_reply_packet;

/** @brief    心跳数据包
  *           心跳数据包的封装结构，通过IP地址判断心跳包目的地址是否跟自己IP地址的相同
  */
typedef struct
{
    char heart_beat_flag;  ///<标志该数据包的类型 H<心跳数据包> 
    char reserved[3];      ///<预留
    char ip_addr[16];      ///<IP地址
}heart_beat_packet;

/** @brief    心跳数据包
  *           心跳数据包的封装结构，通过IP地址判断心跳包目的地址是否跟自己IP地址的相同
  */
typedef struct
{
    pthread_mutex_t *arg_lock;
    int sock;
    char IP[16];
    int port;
    int *quit_flag;
}pthread_arg;

/** @brief    续传文件包
  */
typedef struct
{
    char file_name[FILE_NAME_MAX_LEN + 1]; ///<文件名
    char resered[3];                       ///<预留
    long file_total_size;                  ///<要升级文件的大小
    int resume_transfer_offset;            ///<续传文件偏移量
    time_t mtime;                          ///<记录升级文件的时间属性
}store_file;

/** @brief    传递结构体
  */
typedef struct
{
    int connfd;                            ///<socket
    struct sockaddr_in server_addr;        ///<服务器信息
    char file_name[FILE_NAME_MAX_LEN + 1]; ///<文件名
}ARG;

#endif // CLIENT_UPDATE_FILE_H_

