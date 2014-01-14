      
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

#define FILE_NAME_MAX_LEN 100
#define BUFFER_SIZE       2048
#define LISTENQ           10

/** @struct __file_packet
 * @brief �ļ����ݰ�
 * �ýṹ���ļ����ݵĽ��з�װ�����ն�ͨ���ýṹ�����ݽ��н���
 */
typedef struct __file_packet
{
    char file_data_flag;    /**<��־���ݰ������� F<�ļ����ݰ�> / E<���һ���ļ����ݰ�> */
    char reserved[3];       /**<Ԥ��*/
    int file_id;            /**<�ļ�ID��*/
    int packet_id;          /**<�ļ����ݰ���ID��*/
    int data_len;           /**<�ļ����ݳ���*/
    char buf[BUFFER_SIZE];  /**<�ļ����ݴ�Ż�����*/
}file_packet;

/** @struct __file_reply_packet
 * @brief �ļ����ݰ���Ӧ�����ݰ�
 * �ͻ���ÿ���յ�һ�����������ļ����ݰ�ʱ��ĸ�һ���ظ���
 * �ýṹ�嶨���˻ظ����ݰ�������
 */
typedef struct __file_reply_packet
{
    char file_reply_flag;   /**<��־�����ݰ������� Y<�ļ����ݰ����ճɹ�>/N <�ļ����ݰ�����ʧ��>*/
    char reserved[3];       /**<Ԥ��*/
    int file_id;            /**<�ļ�ID��*/
    int packet_id;          /**<��ʾ���յ����ļ����ݰ���ID��*/
}file_reply_packet;

/** @struct __update_request_packet
 * @brief �����������ݰ�
 * �����������ݰ���Ϣ��װ�ṹ���������ݰ��õ�����
 * 
 */
typedef struct __update_request_packet
{
    char update_request_flag;               /**<��־�����ݰ������� U<�����������ݰ�> */
    char resume_transfer_flag;              /**<�ϵ�������־ G <����>/ T <��ͷ��ʼ��>*/
    char reserved1[2];                       /**<Ԥ��*/
    int resume_transfer_offset;             /**<�ϵ�����ƫ����<���ѽ��յ����ļ����ݵ��ֽ�����ʾ>*/
    time_t mtime;                           /**<��¼�����ļ���ʱ������*/
    char file_name[FILE_NAME_MAX_LEN + 1];  /**<�ļ����洢������*/
    char reserved2[3];                       /**<Ԥ��*/
}update_request_packet;

/** @struct __update_reply_packet
 * @brief ��������Ļظ����ݰ�
 * �������ظ��ͻ�����������ȷ�Ͽɷ�����
 */
typedef struct __update_reply_packet
{
    char update_reply_flag;                 /**<��־�����ݰ������� Y<������������������> / N<����������> / G<��������> */
    char reserved1[3];                       /**<Ԥ��*/
    time_t mtime;                           /**<��¼�����ļ���ʱ������*/
    long file_total_size;                   /**<Ҫ�����ļ��Ĵ�С*/
    char file_name[FILE_NAME_MAX_LEN + 1];  /**<�ļ����洢������*/
    char reserved2[3];                       /**<Ԥ��*/
}update_reply_packet;

/** @struct __heart_beat_packet
 * @brief �������ݰ�
 * �������ݰ��ķ�װ�ṹ��ͨ��IP��ַ�ж�������Ŀ�ĵ�ַ�Ƿ���Լ�IP��ַ����ͬ
 */
typedef struct __heart_beat_packet
{
    char heart_beat_flag;   /**<��־�����ݰ������� H<�������ݰ�> */
    char reserved[3];       /**<Ԥ��*/
    char ip_addr[16];       /**<IP��ַ*/
}heart_beat_packet;

/** @struct __heart_beat_packet
* @brief �������ݰ�
* �������ݰ��ķ�װ�ṹ��ͨ��IP��ַ�ж�������Ŀ�ĵ�ַ�Ƿ���Լ�IP��ַ����ͬ
*/
typedef struct __pthread_arg
{
    pthread_mutex_t *arg_lock;
    int sock;
    char IP[16];
    int port;
    int *quit_flag;
}pthread_arg;

extern unsigned short g_server_port;

#endif
