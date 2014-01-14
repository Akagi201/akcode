      
/** @file server_tool.c
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


#include "global.h"

int judge_cmd(int argc, char **argv);
void usage(void);
int send_data(int sock, char *buf, int buf_len, pthread_mutex_t *sock_lock);
int recv_data(int sock, char *buf, int buf_len, pthread_mutex_t *sock_lock);
int get_file_attr(int fd, struct stat *file_stat);
int stop_update(int sock, pthread_mutex_t *sock_lock);
void *heard_beat_thread(void *arg);
void *handle_tcp_connection(void *arg);
int judge_cmd(int argc, char **argv);
int send_data(int sock, char *buf, int buf_len, pthread_mutex_t *sock_lock);
int recv_data(int sock, char *buf, int buf_len, pthread_mutex_t *sock_lock);
int get_file_attr(int fd, struct stat *file_stat);
int stop_update(int sock, pthread_mutex_t *sock_lock);

unsigned short g_server_port = 6666;  // ��������Ĭ�ϼ����˿ںţ���ͨ�������в����޸�


/** @brief    ��������������
  * @param[in]  argc : �����в�������
  * @param[in]  argv : �����в����ַ�������
  * @param[out]  
  * @return  0 : success -1 : fail
  */
int main(int argc,char **argv)
{
    int lsn_sock = -1;
    int con_sock = -1;
    struct sockaddr_in client_addr, server_addr;
    int addrlen = sizeof(server_addr);
    int ret = -1;

    pthread_t ntid;
    pthread_arg connt_thread_arg;
    char *ip = NULL;
    int quit_flag = 1;
	signal(SIGPIPE, SIG_IGN);

    if(judge_cmd(argc, argv))
    {
        usage();
        return -1;
    }

    lsn_sock = socket(AF_INET,SOCK_STREAM,0);
    if(lsn_sock < 0)
    {
        printf("Create socket fail!\n");
        goto err0;
    }

    memset(&server_addr,0,addrlen);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(g_server_port);
    
    if(bind(lsn_sock, (struct sockaddr *)&server_addr, addrlen) < 0)
    {
        printf("Socket bind() fail!\n");
        goto err0;
    }
    printf("bind() success!\n");
    
    if(listen(lsn_sock, LISTEN_MAX) < 0)
    {
        printf("Socket listen() fail!\n");
        goto err0;
    }
    printf("listening...\n");
    
    while(1)
    {
        memset(&client_addr,0,addrlen);
        printf("\nwaiting for connecting...\n");
        con_sock = accept(lsn_sock, (struct sockaddr *)&client_addr, &addrlen);
        if(con_sock < 0)
        {
            printf("Server accept error!\n");
            break;
        }
        connt_thread_arg.sock = con_sock;
        ip = inet_ntoa(client_addr.sin_addr);
        memcpy(connt_thread_arg.IP, ip, strlen(ip));
        connt_thread_arg.IP[strlen(ip)] = '\0';
        connt_thread_arg.port = (int)client_addr.sin_port;
        printf("\nconnect from client: %s:%d.\n", connt_thread_arg.IP, connt_thread_arg.port);
        connt_thread_arg.quit_flag = &quit_flag;
        if(pthread_create(&ntid, NULL, handle_tcp_connection, (void *)(&connt_thread_arg)) < 0)
        {
            printf("Create thread fail!\n");
            break;
        }
    }

err0:
    quit_flag = 0;
    close(con_sock);
    return 0;
}

/** @brief    �����������̺߳���
  * @param[in]  �̲߳���
  * @param[out]  
  * @return  NULL : fail��((void *)1) : success
  */
void *heard_beat_thread(void *arg)
{
    assert(arg);
    pthread_arg heart_beat_thread_arg = *(pthread_arg *)arg;
    int ret = -1;

    heart_beat_packet heart_beat_pack;
    heart_beat_pack.heart_beat_flag = 'H';
    memcpy(heart_beat_pack.ip_addr, heart_beat_thread_arg.IP, strlen(heart_beat_thread_arg.IP));
    heart_beat_pack.ip_addr[strlen(heart_beat_thread_arg.IP)] = '\0';
    bzero(heart_beat_pack.reserved, sizeof(heart_beat_pack.reserved));

    while(*(heart_beat_thread_arg.quit_flag))
    {
        ret = send_data(heart_beat_thread_arg.sock, (char *)&heart_beat_pack,
        sizeof(heart_beat_packet), heart_beat_thread_arg.arg_lock);
        if (ret < 0)
        {
            printf("Send heart beat fail!\n");
            return NULL;
        }
        sleep(5);//ÿ5�뷢��һ��������
    }
    printf("\n%s-heart beat thread exits!\n", heart_beat_thread_arg.IP);
    return ((void *)1);
}


/** @brief    ����ͻ����������̺߳���
  * @param[in]  �̲߳���
  * @param[out]  
  * @return  NULL������ַָ�� : success ((void *)-1) : fail
  */
void *handle_tcp_connection(void *arg)
{
    assert(arg);
    pthread_arg connet_thread_arg = *(pthread_arg *)arg;
    int sock = connet_thread_arg.sock;

    int send_size_per_time = sizeof(file_packet);
    int read_count = 0;
    int packet_id = 0;
    int ret = -1;

    int fd = -1;
    struct stat file_attr;

    file_packet file_pack;
    file_reply_packet file_reply_pack;
    update_request_packet update_request_pack;
    update_reply_packet update_reply_pack;

    char *recv_buffer = (char *)malloc(sizeof(update_request_packet));
    char *send_buffer = (char *)malloc(send_size_per_time);
    memset(recv_buffer, 0, sizeof(update_request_packet));
    memset(send_buffer, 0, send_size_per_time);
    int file_offset = 0;

    pthread_t heart_beat_tid;
    int heart_beat_thread_quit = 1;//0��ʾ�˳� 1��ʾ��������
    pthread_arg heard_beat_thread_arg = connet_thread_arg;
    pthread_mutex_t send_lock;
    pthread_mutex_t recv_lock;

    pthread_mutex_init(&send_lock,NULL);
    pthread_mutex_init(&recv_lock,NULL);
    heard_beat_thread_arg.arg_lock = &send_lock;
    heard_beat_thread_arg.quit_flag = &heart_beat_thread_quit;

    //���������������߳�
    if(pthread_create(&heart_beat_tid, NULL, heard_beat_thread, (void *)&heard_beat_thread_arg) < 0)
    {
        printf("Create heart beat thread fail!\n");
        goto err0;
    }

    ret = recv_data(sock, recv_buffer, sizeof(update_request_packet), &recv_lock);
    if(ret < 0)
    {
        printf("Receive heart beat thread fail��\n");
        goto err0;
    }

    if (recv_buffer[0] == 'U')
    {
        memcpy((void *)&update_request_pack, (void *)recv_buffer, sizeof(update_request_packet));
            //�ж��ļ����Ƿ����Ҫ��
        if(((strlen(update_request_pack.file_name)) > FILE_NAME_MAX_LEN) || ((strlen(update_request_pack.file_name))  == 0))
        {
            printf("Update file name is wrong!\n");
            goto err1;
        }
            //�ж�Ҫ�����ļ��Ƿ����
        if((access (update_request_pack.file_name, F_OK)) != 0)
        {
            printf("update file:%s doesn't exist!\n", update_request_pack.file_name);
            goto err1;
        }
        fd = open(update_request_pack.file_name, O_RDONLY);
        if (fd < 0)
        {
            printf("Open file:%s fail!\n", update_request_pack.file_name);
            goto err1;
        }
            //��ȡҪ�����ļ����ļ�����
        if(get_file_attr(fd, &file_attr) < 0)
        {
            printf("get file attribute fail!\n");
            goto err1;
        }
        //printf("�ļ�����st_mtime = %s\n", ctime(&file_attr.st_mtime));
        if ((update_request_pack.resume_transfer_flag == 'G')//G��ʾ�ϴ���;ʧ�ܣ��˴ζϵ�����
                && (file_attr.st_mtime == update_request_pack.mtime))//�ж��ļ����޸�ʱ���Ƿ���ͬ
        {
            file_offset = update_request_pack.resume_transfer_offset;
            update_reply_pack.update_reply_flag = 'G';//�ظ�G��ʾ�˴�Ϊ�ϵ�����
        }
        else
        {
            update_reply_pack.update_reply_flag = 'Y';//�ظ�Y��ʾ���¿�ʼ�����ļ�����
        }
        update_reply_pack.file_total_size = file_attr.st_size;
        update_reply_pack.mtime = file_attr.st_mtime;
        memcpy(update_reply_pack.file_name, update_request_pack.file_name, strlen(update_request_pack.file_name));
        update_reply_pack.file_name[strlen(update_request_pack.file_name)] = '\0';

        ret = send_data(sock, (char *)&update_reply_pack, sizeof(update_reply_packet), &send_lock);
        if (ret < 0)
        {
            printf("Reply update request fail!\n");
            goto err1;
        }
    }
    else
    {
        printf("Received update file is wrong.\n");
        goto err1;
    }

    while (*(connet_thread_arg.quit_flag))
    {
        usleep(10);
        if(lseek(fd, file_offset, SEEK_SET) < 0)
        {
            printf("lseek fails!\n");
            goto err1;
        }
        read_count = read(fd, ((file_packet *)send_buffer)->buf, BUFFER_SIZE);
        if(read_count < 0)
        {
            printf("Read file data fails!\n");
            goto err1;
        }

        if ((file_offset + read_count) == update_reply_pack.file_total_size)
        {
            ((file_packet *)send_buffer)->file_data_flag = 'E';//E��ʾ���һ���ļ����ݰ�
        }
        else
        {
            ((file_packet *)send_buffer)->file_data_flag = 'F';//F��ʾ�ļ����ݰ�
        }
        ((file_packet *)send_buffer)->data_len = read_count;
        ((file_packet *)send_buffer)->packet_id = ++packet_id;

SEND_AGAIN:
        ret = send_data(sock, send_buffer, send_size_per_time,&send_lock);
        if(ret < 0)
        {
            printf("Send file data fails!\n");
            goto err1;
        }

        memset(recv_buffer, 0, sizeof(file_reply_packet));
        ret = recv_data(sock, recv_buffer, sizeof(file_reply_packet), &recv_lock);
        if(ret < 0)
        {
            printf("Receive file reply data fail!\n");
            goto err1;
        }

        memcpy((char *)&file_reply_pack, recv_buffer, sizeof(file_reply_packet));
        switch (file_reply_pack.file_reply_flag)
        {
            case 'Y'://���ͳɹ�
                file_offset += read_count;
                printf("Client IP:%16s:%d, Update progress:%d%%\r", connet_thread_arg.IP, connet_thread_arg.port, (file_offset) *100 /(update_reply_pack.file_total_size));
                fflush(stdout);
                if(((file_packet *)send_buffer)->file_data_flag == 'E')
                {
                    printf("\n Client IP:%16s, Update finished!\n", connet_thread_arg.IP);
                    goto err0;
                }
                break;
            case 'N'://����ʧ�ܣ��ط������ݰ�
                goto SEND_AGAIN;
                break;
            default:
                goto err1;
        }
    }

err1:
    stop_update(sock, &send_lock);//����������ֹ���ݰ�
err0:
    heart_beat_thread_quit = 0;//�������߳��˳�
    pthread_mutex_destroy(&send_lock);
    pthread_mutex_destroy(&recv_lock);
    free(recv_buffer);
    free(send_buffer);
    close(sock);
    return (void *)ret;
}

/** @brief    �ж������в�����������ȷ���Ƿ����Ĭ��ֵ
  * @param[in]  argc : �����в�������
  * @param[in]  argv : �����в����ַ�������
  * @param[out]  
  * @return  0 : success -1 : error
  */
int judge_cmd(int argc, char **argv)
{
    if(argc == 1)
    {
        printf("Use default listen port��%d\n", g_server_port);
        usage();
    }
    else if (argc == 2)
    {
        g_server_port = (unsigned short)atoi(argv[1]);
        if (g_server_port == 0)
        {
            printf("port is illigal!\n");
            return -1;
        }
        printf("Use listen port��%d\n", g_server_port);
    }
    else
    {
        printf("Use error!\n");
        return -1;
    }
    return 0;
}

/** @brief    ��ӡ��������ȷʹ�÷���
  * @param[in]  
  * @param[out]  
  * @return  
  */
void usage(void)
{
    printf("Usage: ./server_tool <listen port(default 6666)>!\n");
    return;
}

/** @brief    �������ݺ���
  * @param[in]  sock : �������ӵ��׽���������
  * @param[in]  buf_len : �������ݵ��ֽ���
  * @param[in]  sock_lock : �׽��ֻ�����
  * @param[in]  buf : �������ݻ�����
  * @param[out]  
  * @return  0 : success -1 : fail
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
            printf("Send error��sended %d/%d\n", buf_len - remain_len, buf_len);
            pthread_mutex_unlock(sock_lock);
            return -1;
        }
        remain_len -= recv_len;
        buf += recv_len;
    }
    pthread_mutex_unlock(sock_lock);
    return 0;
}

/** @brief    �������ݺ���
  * @param[in]  sock : �������ӵ��׽���������
  * @param[in]  buf_len : �������ݵ��ֽ���
  * @param[in]  sock_lock : �׽��ֻ�����
  * @param[in]  buf : �������ݻ�����
  * @param[out]  
  * @return  0 : success -1 : fail
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
            printf("Receive error��received %d/%d\n", buf_len - remain_len, buf_len);
            pthread_mutex_unlock(sock_lock);
            return -1;
        }
        remain_len -= recv_len;
        buf += recv_len;
    }
    pthread_mutex_unlock(sock_lock);
    return 0;
}

/** @brief    ��ȡ�ļ�����
  * @param[in]  fd �ļ�������
  * @param[out]  file_stat �ļ����Դ�Žṹ��ָ��
  * @return  0 : success -1 : fail
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

/** @brief    ����ֹͣ���������ݰ����ͻ���
  * @param[in]  sock �������ӵ��׽���������
  * @param[in]  sock_lock �׽��ֻ�����
  * @param[out]  
  * @return  0 : success -1 : fail
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
        printf("Send update reply fail!\n");
        return ret;
    }
    return 0;
}

