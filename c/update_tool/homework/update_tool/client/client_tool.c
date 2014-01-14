      
/** @file client_tool.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    客户端升级程序
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

void *update_thread(void *update_arg);
void *heart_beat_thread(void * heartbeat_arg);
int write_file_info(void);
int read_file_info(char file_name[]);
int recv_file_pack(int connectfd, char *buffer);
void update_process(int connectfd, struct sockaddr_in server, char file_name[]);
int send_data(int sock, char *buf, int buf_len);
int recv_data(int sock, char *buf, int buf_len);
int unpack_file(int fd);
int convert_data(char *p_src, char *p_dst, int len);
void usage(void);

store_file store_file_info;
update_reply_packet update_reply_pack;
heart_beat_packet heart_beat_pack;
time_t start_time;
time_t end_time;
char path_name[PATH_NAME_MAX_LEN] = "dav";

/** @brief    客户端升级主函数
  * @param[in]  argc : 命令行参数个数
  * @param[in]  argv : 命令行参数字符串数组
  * @param[out]  
  * @return  0 : success -1 : fail
  */

int main(int argc, char **argv)
{
    pthread_t update_thread_tid;
    pthread_t heartbeat_thread_tid;
    ARG update_arg;
    ARG heartbeat_arg;

    struct sockaddr_in client_addr;
    
    bzero(&client_addr,sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = htons(INADDR_ANY);
    client_addr.sin_port = htons(0);   // 自动自动分配空闲端口

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0)
    {
        printf("Create socket fail!\n");
        exit(1);
    }
    update_arg.connfd = client_socket;

	// 命令行参数判断
    if (3 == argc)
    {
        bzero(update_arg.file_name,sizeof(update_arg.file_name));
        strcpy(update_arg.file_name, "digicap.dav");
        printf("Use default filename : %s!\n", update_arg.file_name);
        printf("Use default pathname : %s!\n", path_name);
    }
    else if (4 == argc)
    {
        memcpy(update_arg.file_name, argv[3], strlen(argv[3]));
        update_arg.file_name[strlen(argv[3])] = '\0';
        printf("Use default pathname : %s!\n", path_name);
    }
    else if (5 == argc)
    {
        memcpy(update_arg.file_name, argv[3], strlen(argv[3]));
        update_arg.file_name[strlen(argv[3])] = '\0';
        printf("Update path name is%s\n", argv[4]);
        if (argv[4][strlen(argv[4]) - 1] == '/')
        {
            argv[4][strlen(argv[4]) - 1] = '\0';
        }
        bzero(path_name,sizeof(path_name));
        strcpy(path_name,argv[4]);
    }

    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    if (inet_aton(argv[1],&server_addr.sin_addr) == 0) 
    {
        printf("Server address is error!\n");
        exit(1);
    }
        
    server_addr.sin_port = htons((uint16_t)atoi(argv[2]));
    memcpy(&update_arg.server_addr, &server_addr, sizeof(server_addr));

    socklen_t server_addr_length = sizeof(server_addr);
    if(connect(client_socket, (struct sockaddr*)&server_addr, server_addr_length) < 0)
    {
        printf("Cannot connect to the server(IP : %s)!\n", argv[1]);
        exit(1);
    }

    /**升级线程*/
    if (pthread_create(&update_thread_tid, NULL, update_thread, (void*)&update_arg) !=0 )
    {
        printf("Create update_thread fail!\n");
        exit(1);
    }

    /**心跳线程*/
    time(&start_time);
    if (pthread_create(&heartbeat_thread_tid, NULL, heart_beat_thread, (void*)&heartbeat_arg) !=0 )
    {
        printf("Create heartbeat_thread fail!\n");
        exit(1);
    }
    pthread_join(update_thread_tid, NULL);
    
    return 0;
}
 
/** @brief    更新本地存储的文件信息
  * @param[in]  
  * @param[out]  
  * @return  0 : success
  */
int write_file_info(void)
{
    int fd;

    if((fd = open("file_packet", O_RDWR|O_CREAT, 0777)) < 0)
    {
        printf("open file packet failed!\r\n");
        exit(1);
    }
    
    if (write(fd, (char *)&store_file_info, sizeof(store_file_info)) != sizeof(store_file_info))
    {
        printf("write file packet failed!\r\n");
        exit(1);
    }
    
    close(fd);
    return 0;
}

/** @brief    读取本地存储的文件信息
  * @param[in]  
  * @param[out]  
  * @return  0 : success
  */
int read_file_info(char file_name[])
{
    int fd;

    if(access("file_packet", F_OK) == 0)
    {
        if((fd = open("file_packet", O_RDWR)) < 0)
        {
            printf("open file packet failed!\r\n");
            exit(1);
        }
    
        if(read(fd, (char *)&store_file_info, sizeof(store_file_info)) != sizeof(store_file_info))
        {
            printf("read file packet failed!\r\n");
            exit(1);
        }
        close(fd);
    }
    else
    {
        store_file_info.resume_transfer_offset = 0;
        memcpy(store_file_info.file_name, file_name, strlen(file_name));
        store_file_info.file_name[ strlen(file_name) ] = '\0';
        store_file_info.file_total_size = 0;
        store_file_info.mtime = 0;
    }
    
    return 0;
}
 
/** @brief    接收文件包
  * @param[in]  connectfd : 用于接收的socket的描述符
  * @param[in]  buffer : 存储缓冲区
  * @param[out]  
  * @return  0 : success
  */
int recv_file_pack(int connectfd, char *buffer)
{
    int fd;
    time_t now;
    int currpos;
    int transfer_file_offset;
    file_packet file_pack;
    file_reply_packet file_reply_pack;

    transfer_file_offset = store_file_info.resume_transfer_offset;

    if((fd = open("update_file", O_RDWR|O_CREAT, 0777)) < 0)
    {
        printf("open update_file failed!\r\n");
        exit(1);
    }
    if (lseek(fd, transfer_file_offset, SEEK_SET) < 0)
    {
        printf("lseek failed!\r\n");
        exit(1);
    }
    if(recv_data(connectfd, &buffer[1], sizeof(file_pack)-1) < 0)
    {
        printf("recv update_file failed!\r\n");

        file_reply_pack.file_reply_flag = 'N';//告诉服务器重新发送此数据包
        if(send(connectfd, (char *)&file_reply_pack, sizeof(file_reply_pack), 0) < 0)
        {
            printf("send file_packet failed!\r\n");
            write_file_info();
            exit(1);
        }
        if((currpos = lseek(fd, 0, SEEK_CUR)) < 0)
        {
            printf("lseek failed!\r\n");
            write_file_info();
            exit(1);
        }

        store_file_info.resume_transfer_offset = currpos;
        write_file_info();
        return -1;
    }
    
    memcpy((void *)&file_pack, buffer, sizeof(file_pack));
    
    if(write(fd, file_pack.buf, file_pack.data_len) != file_pack.data_len)
    {
        printf("write update_file failed!\r\n");
    write_file_info();
        exit(1);
    }
    usleep(100);//写入flash
    store_file_info.resume_transfer_offset += file_pack.data_len;
    file_reply_pack.file_reply_flag = 'Y';
    if(send_data(connectfd, (char *)&file_reply_pack, sizeof(file_reply_pack)) < 0)
    {
        printf("send file_packet failed!\r\n");
        write_file_info();
        exit(1);
    }
    printf("The update progress:%d%\r", store_file_info.resume_transfer_offset * 100/(store_file_info.file_total_size));
    fflush(stdout);
    if(file_pack.file_data_flag == 'E')
    {
        
        unpack_file(fd); 
        printf("Update finished!\r\n");
        sleep(2);
    }

    close(fd);
    return 0;
}


/** @brief    解析文件包头
  * @param[in]  文件描述符
  * @param[out]  
  * @return  0 : success
  */
int unpack_file(int fd)
{

    char buffer[store_file_info.file_total_size + 1];
    FIRMWARE_HEADER fir1, fir2;
    
    bzero(buffer, sizeof(buffer));
    lseek(fd, 0, SEEK_SET);
    read(fd, &fir1, sizeof(FIRMWARE_HEADER));
    convert_data((char*)&fir1, (char*)&fir2, sizeof(FIRMWARE_HEADER));

    typedef struct
    {
        FIRMWARE_HEADER firmware;
        UPGRADE_FILE_HEADER fileheader[fir2.file_nums];
    }FirmUpdateHeader;
    unsigned char* p = NULL;
    int i, j;
    unsigned int sum = 0;
    FirmUpdateHeader firm_header1, firm_header2;
    char sbuf[100];
    int file_fd;

    lseek(fd, 0, SEEK_SET);
    read(fd, &firm_header1, sizeof(FirmUpdateHeader));
    convert_data((char*)&firm_header1, (char*)&firm_header2, sizeof(FirmUpdateHeader));
    for (i = 0; i < firm_header2.firmware.file_nums; i++)
    {   
        printf("%s file_size is : %d\n", firm_header2.fileheader[i].file_name, firm_header2.fileheader[i].file_len);
    }
    p = (unsigned char*)&firm_header2.firmware.file_nums;
    for (i = 0; i < firm_header2.firmware.header_length - 12; i++)
    {
        sum += *p;
        p++;
    }
    if (sum == firm_header2.firmware.header_checksum)
    {
        printf("Firmware checksum success!\n");
    }
    else
    {
        printf("Firmware checksum fail！\n");
        return -1;
    }
    
    lseek(fd, 0, SEEK_SET);
    read(fd, buffer,  store_file_info.file_total_size);
    sum = 0;
    for (i = 0; i < firm_header2.firmware.file_nums; i++)
    {
        p = &buffer[firm_header2.fileheader[i].start_offset];
        for (j = 0; j < firm_header2.fileheader[i].file_len; j++)
        {
            sum += *p;
            p++;
        }
        if (sum == firm_header2.fileheader[i].checksum)
        {
            sum = 0;
            p = &buffer[firm_header2.fileheader[i].start_offset];      //重新定位到buffer数组的每个文件的偏移处
            bzero(sbuf,sizeof(sbuf));
            sprintf(sbuf,"%s/%s", path_name, firm_header2.fileheader[i].file_name);
            if (access(sbuf, F_OK) < 0)
            {
                printf("File: %s doesn't exist, please recreate！\n", firm_header2.fileheader[i].file_name);
                if ((file_fd = open(sbuf, O_RDWR | O_CREAT, 0777)) == -1)
                {
                    close(file_fd);
                    perror("open");
                    return -1;
                }
                lseek(file_fd, 0, SEEK_SET);
                if (write(file_fd, p, firm_header2.fileheader[i].file_len) == -1)
                {
                    close(file_fd);
                    perror("write");
                    return -1;
                }
                close(file_fd);
            }
            else
            {
                printf("File: %s exists,just overwrite it！\n", firm_header2.fileheader[i].file_name);
                if ((file_fd = open(sbuf, O_RDWR | O_CREAT, 0777)) == -1)
                {
                    close(file_fd);
                    perror("open");
                    return -1;
                }   
                lseek(file_fd, 0, SEEK_SET);
                if (write(file_fd, p, firm_header2.fileheader[i].file_len) == -1)
                {
                    close(file_fd);
                    perror("write");
                    return -1;
                }
                close(file_fd);
            }   
        }
        else
        {
            printf("%s checksum fail！\n", firm_header2.fileheader[i].file_name);
            return -1;  
        }
    }
}

/** @brief    处理文件核心部分
  * @param[in]  connectfd : 用于连接的socket描述符
  * @param[in]  server : 服务器地址
  * @param[in]  file_name : 升级文件名
  * @param[out]  
  * @return  0 : success
  */
void update_process(int connectfd, struct sockaddr_in server, char file_name[])
{
    update_request_packet update_request_pack;
    int buffer_size = sizeof(file_packet);
    char buffer[buffer_size];
    bzero(buffer, buffer_size);

    read_file_info(file_name);

    update_request_pack.update_request_flag = 'U';
    if(store_file_info.resume_transfer_offset == 0)
    {
        update_request_pack.resume_transfer_flag = 'T';
    }
    else
    {
        update_request_pack.resume_transfer_flag = 'G';
    }
    
    update_request_pack.resume_transfer_offset = store_file_info.resume_transfer_offset;
    memcpy(update_request_pack.file_name, store_file_info.file_name, sizeof(store_file_info.file_name));
    update_request_pack.mtime = store_file_info.mtime;

    if(send_data(connectfd, (char *)&update_request_pack, sizeof(update_request_pack)) < 0)
    {
        printf("send failed!\n");
        exit(1);
    }

    while(1)
    {
        bzero(buffer, BUFFER_SIZE);
        if(recv_data(connectfd, buffer, 1) < 0)
        {
            printf("recv flag failed!\n");
            exit(1);
        }

        switch(buffer[0])
        {
            case 'Y':
                store_file_info.resume_transfer_offset = 0;
            case 'G':
                if(recv_data(connectfd, &buffer[1], sizeof(update_reply_pack)-1) < 0)
                {
                    printf("recv update_reply_pack failed!\n");
                    exit(1);
                }
                memcpy((void *)&update_reply_pack, buffer, sizeof(update_reply_pack));
                memcpy(store_file_info.file_name, update_reply_pack.file_name, sizeof(update_reply_pack.file_name));
                store_file_info.file_total_size = update_reply_pack.file_total_size;
                store_file_info.mtime =update_reply_pack.mtime;
                write_file_info();
                break;
            case 'H':
                if(recv_data(connectfd, &buffer[1], sizeof(heart_beat_pack)-1) < 0)
                {
                    printf("recv heart_beat_pack failed!\n");
                    write_file_info();
                    exit(1);
                }
                time(&start_time);
                break;
            case 'F':
                if(recv_file_pack(connectfd, buffer) < 0)
                {
                    printf("update failed!\n");
                            continue;
                }
                        break;
            case 'E':
                if(recv_file_pack(connectfd, buffer) < 0)
                {
                    printf("update failed!\r\n");
                    continue;
                }
                store_file_info.resume_transfer_offset = 0;
                write_file_info();
                exit(1);
            default:
                write_file_info();
                printf("update request fail!\r\n");
                exit(1);
        }
    }
    close(connectfd);
}
 
/** @brief    升级线程函数
  * @param[in]  传递结构体指针
  * @param[out]  
  * @return  
  */
void *update_thread(void *update_arg)
{
    ARG info;
    info = *(ARG *)update_arg;

    update_process(info.connfd, info.server_addr, info.file_name);
    pthread_exit(NULL);
}
 
/** @brief    心跳线程函数
  * @param[in]  传递结构体指针
  * @param[out]  
  * @return  
  */
void *heart_beat_thread(void *heartbeat_arg)
{
    while(1)
    {
        time(&end_time);
        if(abs(end_time-start_time) > 10)
        {
            printf("Time outs: update fail!\n");
            write_file_info();
            exit(1);
        }
        sleep(1);
    }
    pthread_exit(NULL);
}

/** @brief    发送数据函数
  * @param[in]  sock : 用于连接的socket描述符
  * @param[in]  buf : 缓冲区
  * @param[in]  buf_len : 缓冲区大小
  * @param[out]  
  * @return  0 : success
  */

int send_data(int sock, char *buf, int buf_len)
{
    int remain_len = buf_len;
    int recv_len = 0;
    while (remain_len > 0)
    {
        recv_len = send(sock, buf, remain_len, 0);
        if (recv_len < 0)
        {
            printf("Send error, sended %d/%d\n", buf_len - remain_len, buf_len);
            return -1;
        }
        remain_len -= recv_len;
        buf += recv_len;
    }
    return 0;
}

/** @brief    接收数据函数
  * @param[in]  sock : 用于连接的socket描述符
  * @param[in]  buf : 缓冲区
  * @param[in]  buf_len : 缓冲区大小
  * @param[out]  
  * @return  0 : success
  */

int recv_data(int sock, char *buf, int buf_len)
{
    int remain_len = buf_len;
    int recv_len = 0;
    while (remain_len > 0)
    {
        recv_len = recv(sock, buf, remain_len, 0);
        if (recv_len < 0)
        {
            printf("Receive error，received %d/%d\n", buf_len - remain_len, buf_len);
            return -1;
        }
        remain_len -= recv_len;
        buf += recv_len;
    }
    return 0;
}
 
/** @brief    利用简单的异或进行数据变换，用于升级文件头的打包和解包
  * @param[in]  char *p_src, 数据的源地址
  * @param[in]  int len，数据的长度
  * @param[out]  char *p_dst, 数据的目的地址
  * @return  0 - 成功，-1 - 失败
  */
int convert_data(char *p_src, char *p_dst, int len)
{
    // 固定的幻数，用于异或变换
    UINT8 magic[] = {0xba, 0xcd, 0xbc, 0xfe, 0xd6, 0xca, 0xdd, 0xd3,
                    0xba, 0xb9, 0xa3, 0xab, 0xbf, 0xcb, 0xb5, 0xbe};
    int i = 0;
    int j = 0;
    int magic_len = 0;
    int start_magic = 0;
    // 参数检查
    if ((NULL == p_src) || (NULL == p_dst))
    {
        return -1;
    }
    magic_len = sizeof(magic);  
    while(i < len)
    {
        // 用start_magic控制每次内循环magic的起始位置
        for (j = 0; (j < magic_len) && (i < len); j++, i++)
        {
            *p_dst++ = *p_src++ ^ magic[(start_magic+j)%magic_len]; // 进行异或变换
        }
        start_magic = (start_magic + 1) % magic_len;
    }   
    return 0;
}


/** @brief    输入出错后，打印方法提示信息
  * @param[in]  
  * @param[out]  
  * @return  
  */
void usage(void)
{
    printf("Usage: ./client_tool <server ip address> <server port> <update_filename> <absolute path or relative path>!\n");
    return;
}

