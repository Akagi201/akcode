      
/** @file interface.c
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

#include "update_tool.h"
#include "interface.h"

/**@fn	
* @brief ��������������У���ʼ���˿ڲ���
* @param[in] argc �����в�������
* @param[in] argv �����в���ָ��
* @return	0��ʾ�ɹ���-1��ʾʧ��
*/
int process_cmdline(int argc, char **argv)
{
    if(argc == 1)
    {
        printf("�˿ںŲ���Ĭ��ֵ��%d\n", g_server_port);
        usage();
    }
    else if (argc == 2)
    {
        g_server_port = (unsigned short)atoi(argv[1]);
        if (g_server_port == 0)
        {
            printf("�˿���������.\n");
            return -1;
        }
        printf("�˿ں�����Ϊ��%d\n", g_server_port);
    }
    else
    {
        printf("ʹ�÷�������\n");
        return -1;
    }
    return 0;
}

/**@fn	
* @brief ��ӡ��������ȷʹ�÷���
* @return	��
*/
void usage(void)
{
    printf("ʹ�÷���: ./tranfer_tool_server <�˿ں�>.\n");
    return;
}

/**@fn	
* @brief �����ƶ��ֽ���������
* @param[in] sock �׽���������
* @param[in] buf_len �������ݵ��ֽ���
* @param[in] sock_lock �׽��ֻ�����
* @param[in] buf �������ݴ�Ż�����
* @return	0��ʾ�ɹ���-1��ʾʧ��
*/
int send_data(int sock, char *buf, int buf_len, pthread_mutex_t *sock_lock)
{
    int remain_len = buf_len;
    int recv_len = 0;
    pthread_mutex_lock(sock_lock);
    while (remain_len > 0)
    {
        recv_len = send(sock, buf, remain_len, 0);
        if (recv_len < 0)
        {
            printf("���ͳ�������%d/%d\n", buf_len - remain_len, buf_len);
            pthread_mutex_unlock(sock_lock);
            return -1;
        }
        remain_len -= recv_len;
        buf += recv_len;
    }
    pthread_mutex_unlock(sock_lock);
    return 0;
}

/**@fn	
* @brief �����ƶ��ֽ���������
* @param[in] sock �׽���������
* @param[in] buf_len �������ݵ��ֽ���
* @param[in] sock_lock �׽��ֻ�����
* @param[out] buf �������ݴ�Ż�����
* @return	0��ʾ�ɹ���-1��ʾʧ��
*/
int recv_data(int sock, char *buf, int buf_len, pthread_mutex_t *sock_lock)
{
    int remain_len = buf_len;
    int recv_len = 0;
    pthread_mutex_lock(sock_lock);
    while (remain_len > 0)
    {
        recv_len = recv(sock, buf, remain_len, 0);
        if (recv_len < 0)
        {
            printf("���ճ�������%d/%d\n", buf_len - remain_len, buf_len);
            pthread_mutex_unlock(sock_lock);
            return -1;
        }
        remain_len -= recv_len;
        buf += recv_len;
    }
    pthread_mutex_unlock(sock_lock);
    return 0;
}

/**@fn	
* @brief ��ȡ�ļ�����
* @param[in] fd �ļ�������
* @param[out] file_stat �ļ����Դ�Žṹ��ָ��
* @return	0��ʾ�ɹ���-1��ʾʧ��
*/
int get_file_attr(int fd, struct stat *file_stat)
{
    long length = 0;
    if (fstat(fd, file_stat) < 0)
    {
        return -1;
    }
    return 0;
}

/**@fn	
* @brief ����ֹͣ���������ݰ����ͻ���
* @param[in] sock �׽���������
* @param[in] sock_lock �׽��ֻ�����
* @return	0��ʾ�ɹ���-1��ʾʧ��
*/
int stop_update(int sock, pthread_mutex_t *sock_lock)
{
    int ret = -1;
    update_reply_packet update_reply_pack;
    memset((void *)&update_reply_pack, 0, sizeof(update_reply_packet));
    update_reply_pack.update_reply_flag = 'N';
    ret = send_data(sock, (char *)&update_reply_pack, sizeof(update_reply_packet), sock_lock);
    if (ret < 0)
    {
        printf("���������ظ�ʧ��!\n");
        return ret;
    }
    return 0;
}
