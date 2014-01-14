      
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
* @brief 处理输入的命令行，初始化端口参数
* @param[in] argc 命令行参数个数
* @param[in] argv 命令行参数指针
* @return	0表示成功，-1表示失败
*/
int process_cmdline(int argc, char **argv)
{
    if(argc == 1)
    {
        printf("端口号采用默认值：%d\n", g_server_port);
        usage();
    }
    else if (argc == 2)
    {
        g_server_port = (unsigned short)atoi(argv[1]);
        if (g_server_port == 0)
        {
            printf("端口输入有误.\n");
            return -1;
        }
        printf("端口号设置为：%d\n", g_server_port);
    }
    else
    {
        printf("使用方法有误。\n");
        return -1;
    }
    return 0;
}

/**@fn	
* @brief 打印函数的正确使用方法
* @return	无
*/
void usage(void)
{
    printf("使用方法: ./tranfer_tool_server <端口号>.\n");
    return;
}

/**@fn	
* @brief 发送制定字节数的数据
* @param[in] sock 套接字描述符
* @param[in] buf_len 发送数据的字节数
* @param[in] sock_lock 套接字互斥锁
* @param[in] buf 发送数据存放缓冲区
* @return	0表示成功，-1表示失败
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
            printf("发送出错，发送%d/%d\n", buf_len - remain_len, buf_len);
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
* @brief 接收制定字节数的数据
* @param[in] sock 套接字描述符
* @param[in] buf_len 接收数据的字节数
* @param[in] sock_lock 套接字互斥锁
* @param[out] buf 接收数据存放缓冲区
* @return	0表示成功，-1表示失败
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
            printf("接收出错，接收%d/%d\n", buf_len - remain_len, buf_len);
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
* @brief 获取文件属性
* @param[in] fd 文件描述符
* @param[out] file_stat 文件属性存放结构体指针
* @return	0表示成功，-1表示失败
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
* @brief 发送停止升级的数据包给客户端
* @param[in] sock 套接字描述符
* @param[in] sock_lock 套接字互斥锁
* @return	0表示成功，-1表示失败
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
        printf("发送升级回复失败!\n");
        return ret;
    }
    return 0;
}
