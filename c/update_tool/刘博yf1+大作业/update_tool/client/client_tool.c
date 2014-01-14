      
/** @file client_tool.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    客户端主程序
  * 
  * @author   liuboyf1
  * @date     2012-9-15
  * @version  V1.0.0
  * 
  * @note ///Description here 
  * @note History:    
  * @note     <author>   <time>    <version >   <desc>
  * @note  
  * @warning  
  */
 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "update_tool.h"
#include "upgrade_file.h"

void *update_thread(void *arg);
void *heart_beat_thread(void * brg);
int write_file_info(void);
int read_file_info(char file_name[]);
int recv_file_pack(int connectfd, char *buffer);
void process_server(int connectfd, struct sockaddr_in server, char file_name[]);
int send_data(int sock, char *buf, int buf_len);
int recv_data(int sock, char *buf, int buf_len);

store_file store_file_info;
update_reply_packet update_reply_pack;
heart_beat_packet heart_beat_pack;
time_t start_time;
time_t end_time;

int main(int argc, char **argv)
{
	pthread_t thread1;
	pthread_t thread2;
	ARG arg;
	ARG brg;

    struct sockaddr_in client_addr;

    bzero(&client_addr,sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = htons(INADDR_ANY);
    client_addr.sin_port = htons(0);

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0)
    {
        printf("Create Socket Failed!\r\n");
        exit(1);
    }
		
	arg.connfd = client_socket;
	memcpy(arg.file_name, argv[3], strlen(argv[3]));
	arg.file_name[strlen(argv[3])] = '\0';

	struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    if (inet_aton(argv[1],&server_addr.sin_addr) == 0) 
    {
        printf("Server IP Address Error!\r\n");
        exit(1);
    }
		
	server_addr.sin_port = htons((uint16_t)atoi(argv[2]));
	memcpy(&arg.server_addr, &server_addr, sizeof(server_addr));

	socklen_t server_addr_length = sizeof(server_addr);
	if(connect(client_socket, (struct sockaddr*)&server_addr, server_addr_length) < 0)
    {
        printf("不能连接到IP地址为:%s的服务器!\r\n", argv[1]);
        exit(1);
    }

	/**升级线程*/
	if (pthread_create(&thread1, NULL, update_thread, (void*)&arg) !=0 )
	{
		printf("creat update_pthread failed!\r\n");
		exit(1);
	}

	/**心跳线程*/
    time(&start_time);
	if (pthread_create(&thread2, NULL, heart_beat_thread, (void*)&brg) !=0 )
	{
		printf("creat heart_beat_pthread failed!\r\n");
		exit(1);
	}
	pthread_join(thread1, NULL);
	
        return 0;
}

/**@fn	store_info	  
 * @brief update of local file packet	  
 * @param[in]  no
 * @param[out] no
 * @return	0  
 */
int write_file_info(void)
{
	int fd;

	if((fd = open("store_file_packet", O_RDWR|O_CREAT, 0777)) < 0)
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

/**@fn	read_file_info	  
 * @brief read local file packet	  
 * @param[in]  no
 * @param[out] no
 * @return	0  
 */
int read_file_info(char file_name[])
{
	int fd;

	if(access("store_file_packet", F_OK) == 0)
	{
		if((fd = open("store_file_packet", O_RDWR)) < 0)
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
		store_file_info.file_name[6] = '\0';
		store_file_info.file_total_size = 0;
		store_file_info.mtime = 0;
	}
	
	return 0;
}

/**@fn	recv_file_pack	  
 * @brief	receive file packet	  
 * @param[in]  connectfd
 * @param[out] no
 * @return	0 
 */
int recv_file_pack(int connectfd, char *buffer)
{
	int fd;
//	int progress_flag = 0;
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

		file_reply_pack.file_reply_flag = 'N';//告诉服务器从新发送此数据包
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
//	progress_flag++;
	file_reply_pack.file_reply_flag = 'Y';
	if(send_data(connectfd, (char *)&file_reply_pack, sizeof(file_reply_pack)) < 0)
	{
		printf("send file_packet failed!\r\n");
        write_file_info();
		exit(1);
	}
	printf("升级进度:%d%\r", store_file_info.resume_transfer_offset * 100/(store_file_info.file_total_size));
    fflush(stdout);
	if(file_pack.file_data_flag == 'E')
	{
		printf("升级完成!\r\n");
        sleep(2);
	}

	close(fd);
	return 0;
}

/**@fn	process_server	  
 * @brief		  
 * @param[in]  connectfd, server
 * @param[out] 
 * @return no  
 */
void process_server(int connectfd, struct sockaddr_in server, char file_name[])
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
		printf("send failed!\r\n");
		exit(1);
	}

	while(1)
	{
		bzero(buffer, BUFFER_SIZE);
		if(recv_data(connectfd, buffer, 1) < 0)
		{
			printf("recv flag failed!\r\n");
            exit(1);
		}

//		printf("%c\r\n", buffer[0]);
		switch(buffer[0])
		{
			case 'Y':
                store_file_info.resume_transfer_offset = 0;
			case 'G':
				if(recv_data(connectfd, &buffer[1], sizeof(update_reply_pack)-1) < 0)
				{
                    printf("recv update_reply_pack failed!\r\n");
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
					printf("recv heart_beat_pack failed!\r\n");
                    write_file_info();
                    exit(1);
				}
//				memcpy((void *)&heart_beat_pack, &buffer, sizeof(heart_beat_pack));
				time(&start_time);
				break;
			case 'F':
				if(recv_file_pack(connectfd, buffer) < 0)
				{
					printf("update failed!\r\n");
//					exit(1);
                    continue;
				}
                break;
            case 'E':
                if(recv_file_pack(connectfd, buffer) < 0)
                {
                    printf("update failed!\r\n");
//					exit(1);
                    continue;
                }
                store_file_info.resume_transfer_offset = 0;
                write_file_info();
                exit(1);
			default:
                write_file_info();
				printf("升级请求失败!\r\n");
                exit(1);
		}
	}
	close(connectfd);
}

/**@fn	*update_thread	  
 * @brief		  
 * @param[in] *arg 
 * @param[out] no
 * @return	no
 */
void *update_thread(void *arg)
{
    ARG info;
    info = *(ARG *)arg;

//	printf("2222\r\n");
    process_server(info.connfd, info.server_addr, info.file_name);
    pthread_exit(NULL);
}

/**@fn *heart_beat_thread	  
 * @brief		  
 * @param[in]  *brg
 * @param[out] no
 * @return	  no
 */
void *heart_beat_thread(void *brg)
{
//	printf("5555\r\n");
    while(1)
    {
	    time(&end_time);
	    if(abs(end_time-start_time) > 10)
	    {
		    printf("超时升级失败!\r\n");
            write_file_info();
            exit(1);
	    }
        sleep(1);
    }
	pthread_exit(NULL);
}




int send_data(int sock, char *buf, int buf_len)
{
    int remain_len = buf_len;
    int recv_len = 0;
    while (remain_len > 0)
    {
        recv_len = send(sock, buf, remain_len, 0);
        if (recv_len < 0)
        {
            printf("发送出错，发送%d/%d\n", buf_len - remain_len, buf_len);
            return -1;
        }
        remain_len -= recv_len;
        buf += recv_len;
    }
    return 0;
}


int recv_data(int sock, char *buf, int buf_len)
{
    int remain_len = buf_len;
    int recv_len = 0;
    while (remain_len > 0)
    {
        recv_len = recv(sock, buf, remain_len, 0);
        if (recv_len < 0)
        {
            printf("接收出错，接收%d/%d\n", buf_len - remain_len, buf_len);
            return -1;
        }
        remain_len -= recv_len;
        buf += recv_len;
    }
    return 0;
}
