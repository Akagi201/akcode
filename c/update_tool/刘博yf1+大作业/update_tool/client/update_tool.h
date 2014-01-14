      
/** @file update_tool.h
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    
  * 
  * @author   liuboyf1
  * @date     2012-9-17
  * @version  V1.0.0
  * 
  * @note ///Description here 
  * @note History:    
  * @note     <author>   <time>    <version >   <desc>
  * @note  
  * @warning  
  */


#ifndef _UPDATE_TOOL_
#define _UPDATE_TOOL_

#define FILE_NAME_MAX_LEN 100
#define BUFFER_SIZE       2048
#define LISTENQ           10

/** @struct __file_packet
* @brief 文件数据包
* 该结构对文件数据的进行封装，接收端通过该结构对数据进行解析
*/
typedef struct __file_packet
{
    char file_data_flag;    /**<标志数据包的类型 F<文件数据包> / E<最后一个文件数据包> */
    char reserved[3];       /**<预留*/
    int file_id;            /**<文件ID号*/
    int packet_id;          /**<文件数据包的ID号*/
    int data_len;           /**<文件数据长度*/
    char buf[BUFFER_SIZE];  /**<文件数据存放缓冲区*/
}file_packet;

/** @struct __file_reply_packet
* @brief 文件数据包的应答数据包
* 客户端每接收到一个服务器的文件数据包时候的给一个回复，
* 该结构体定义了回复数据包的内容
*/
typedef struct __file_reply_packet
{
    char file_reply_flag;   /**<标志该数据包的类型 Y<文件数据包接收成功>/N <文件数据包接收失败>*/
    char reserved[3];       /**<预留*/
    int file_id;            /**<文件ID号*/
    int packet_id;          /**<表示接收到的文件数据包的ID号*/
}file_reply_packet;

/** @struct __update_request_packet
* @brief 升级请求数据包
* 升级请求数据包信息封装结构，发送数据包得到请求
* 
*/
typedef struct __update_request_packet
{
    char update_request_flag;               /**<标志该数据包的类型 U<升级请求数据包> */
    char resume_transfer_flag;              /**<断点续传标志 G <续传>/ T <从头开始传>*/
    char reserved1[2];                       /**<预留*/
    int resume_transfer_offset;             /**<断点续传偏移量<用已接收到的文件数据的字节数表示>*/
    time_t mtime;                           /**<记录升级文件的时间属性*/
    char file_name[FILE_NAME_MAX_LEN + 1];  /**<文件名存储缓冲区*/
    char reserved2[3];                       /**<预留*/
}update_request_packet;

/** @struct __update_reply_packet
* @brief 升级请求的回复数据包
* 服务器回复客户端升级请求，确认可否升级
*/
typedef struct __update_reply_packet
{
    char update_reply_flag;                 /**<标志该数据包的类型 Y<可以升级，不可续传> / N<不可以升级> / G<续传升级> */
    char reserved1[3];                       /**<预留*/
    time_t mtime;                           /**<记录升级文件的时间属性*/
    long file_total_size;                   /**<要升级文件的大小*/
    char file_name[FILE_NAME_MAX_LEN + 1];  /**<文件名存储缓冲区*/
    char reserved2[3];                       /**<预留*/
}update_reply_packet;

/** @struct __heart_beat_packet
* @brief 心跳数据包
* 心跳数据包的封装结构，通过IP地址判断心跳包目的地址是否跟自己IP地址的相同
*/
typedef struct __heart_beat_packet
{
    char heart_beat_flag;   /**<标志该数据包的类型 H<心跳数据包> */
    char reserved[3];       /**<预留*/
    char ip_addr[16];       /**<IP地址*/
}heart_beat_packet;


/**续传文件包*/
typedef struct __store_file
{
	char file_name[FILE_NAME_MAX_LEN + 1];  /**<文件名*/
    char resered[3];                        /**<预留*/
	long file_total_size;                   /**<要升级文件的大小*/
	int resume_transfer_offset;             /**续传文件偏移量*/
	time_t mtime;                           /**<记录升级文件的时间属性*/
}store_file;

/**传递结构体*/
typedef struct _ARG
{
	int connfd;                             /**socket*/
	struct sockaddr_in server_addr;         /**服务器信息*/
	char file_name[FILE_NAME_MAX_LEN + 1];  /**<文件名*/
}ARG;

#endif
