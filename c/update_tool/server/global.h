      
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

#if defined (__cplusplus) || defined(c_plusplus)
}
#endif


#define FILE_NAME_MAX_LEN 100
#define PATH_NAME_MAX_LEN 200
#define BUFFER_SIZE       8*1024
#define LISTEN_MAX           10

extern unsigned short g_server_port;

/** @brief    �����������ı���ͷ
  */
typedef struct
{
    unsigned int magic_number;      ///< ħ���֣�0x484b5753
    unsigned int header_checksum;   ///< ����ͷУ���
    unsigned int header_length;     ///< ����ͷ����
    unsigned int file_nums;         ///< �������ļ�����
    unsigned int language;          ///< ���������ԣ�1-Ӣ�ģ�2-����
    unsigned int device_class;      ///< 1 �CDS9000 DVR, 2 - IPC
    unsigned int oem_code;          ///< 1 �Chikvision
    char            res[36];            ///< �����ֶ�
}FIRMWARE_HEADER;

/** @brief    ÿ�������ļ��ı���ͷ
  */
typedef struct
{
    char file_name[32]; ///< �ļ���
    int  start_offset;  ///< �ļ���ʼλ��(���������е�ƫ����)
    int  file_len;      ///< �ļ�����
    int  checksum;      ///< �����ļ���У��ͣ��������ñ���ͷ
}UPGRADE_FILE_HEADER;
 
/** @brief    �ļ����ݰ�
  *           �ýṹ���ļ����ݵĽ��з�װ�����ն�ͨ���ýṹ�����ݽ��н���
  */
typedef struct
{
    char file_data_flag;   ///<��־���ݰ������� F<�ļ����ݰ�> / E<���һ���ļ����ݰ�> 
    char reserved[3];      ///<Ԥ��
    int file_id;           ///<�ļ�ID��
    int packet_id;         ///<�ļ����ݰ���ID��
    int data_len;          ///<�ļ����ݳ���
    char buf[BUFFER_SIZE]; ///<�ļ����ݴ�Ż�����
}file_packet;

/** @brief    �ļ����ݰ���Ӧ�����ݰ�
  *           �ͻ���ÿ���յ�һ�����������ļ����ݰ�ʱ��ĸ�һ���ظ�
  */
typedef struct
{
    char file_reply_flag;  ///<��־�����ݰ������� Y<�ļ����ݰ����ճɹ�>/N <�ļ����ݰ�����ʧ��>
    char reserved[3];      ///<Ԥ��
    int file_id;           ///<�ļ�ID��
    int packet_id;         ///<��ʾ���յ����ļ����ݰ���ID��
}file_reply_packet;

/** @brief    �����������ݰ�
  *           �����������ݰ���Ϣ��װ�ṹ���������ݰ��õ�����
  */
typedef struct
{
    char update_request_flag;               ///<��־�����ݰ������� U<�����������ݰ�> 
    char resume_transfer_flag;              ///<�ϵ�������־ G <����>/ T <��ͷ��ʼ��>
    char reserved1[2];                      ///<Ԥ��
    int resume_transfer_offset;             ///<�ϵ�����ƫ����<���ѽ��յ����ļ����ݵ��ֽ�����ʾ>
    time_t mtime;                           ///<��¼�����ļ���ʱ������
    char file_name[FILE_NAME_MAX_LEN + 1];  ///<�ļ����洢������
    char reserved2[3];                      ///<Ԥ��
}update_request_packet;

/** @brief    ��������Ļظ����ݰ�
  *           �������ظ��ͻ�����������ȷ�Ͽɷ�����
  */
typedef struct
{
    char update_reply_flag;                 ///<��־�����ݰ������� Y<������������������> / N<����������> / G<��������> 
    char reserved1[3];                      ///<Ԥ��
    time_t mtime;                           ///<��¼�����ļ���ʱ������
    long file_total_size;                   ///<Ҫ�����ļ��Ĵ�С
    char file_name[FILE_NAME_MAX_LEN + 1];  ///<�ļ����洢������
    char reserved2[3];                      ///<Ԥ��
}update_reply_packet;

/** @brief    �������ݰ�
  *           �������ݰ��ķ�װ�ṹ��ͨ��IP��ַ�ж�������Ŀ�ĵ�ַ�Ƿ���Լ�IP��ַ����ͬ
  */
typedef struct
{
    char heart_beat_flag;  ///<��־�����ݰ������� H<�������ݰ�> 
    char reserved[3];      ///<Ԥ��
    char ip_addr[16];      ///<IP��ַ
}heart_beat_packet;

/** @brief    �������ݰ�
  *           �������ݰ��ķ�װ�ṹ��ͨ��IP��ַ�ж�������Ŀ�ĵ�ַ�Ƿ���Լ�IP��ַ����ͬ
  */
typedef struct
{
    pthread_mutex_t *arg_lock;
    int sock;
    char IP[16];
    int port;
    int *quit_flag;
}pthread_arg;

/** @brief    �����ļ���
  */
typedef struct
{
    char file_name[FILE_NAME_MAX_LEN + 1]; ///<�ļ���
    char resered[3];                       ///<Ԥ��
    long file_total_size;                  ///<Ҫ�����ļ��Ĵ�С
    int resume_transfer_offset;            ///<�����ļ�ƫ����
    time_t mtime;                          ///<��¼�����ļ���ʱ������
}store_file;

/** @brief    ���ݽṹ��
  */
typedef struct
{
    int connfd;                            ///<socket
    struct sockaddr_in server_addr;        ///<��������Ϣ
    char file_name[FILE_NAME_MAX_LEN + 1]; ///<�ļ���
}ARG;

#endif // CLIENT_UPDATE_FILE_H_

