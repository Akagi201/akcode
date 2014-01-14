/**@file  client_recv_file.c
 * @note  HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief 客户端中的升级文件接收的各个函数定义 
 *
 * @author		yanghongwen
 * @date		2012/09/14
 * @version  V1.0
 *
 * @note ///Description here 
 * @note History:        
 * @note     <author>   <time>    <version >   <desc>
 * @note  
 * @warning 
 */

#include "client_recv_file.h"
#include "client_heart.h" 
                      

/**@brief          初始化套接字
 * @param port     端口号
 * @param p_sock   套接字回传
 * @return         0:right, -1:error
 */
int client_sock_init(char *p_ip_addr, int port, int *sock, struct sockaddr_in *s_addr)
{	
	int err = 0;
	int cli_sock = 0;	

    cli_sock = socket(AF_INET, SOCK_STREAM, 0);      
    if( cli_sock < 0)
    {
        printf("Create Socket Failed!\n");
        exit(1);
    } 
    
    bzero(s_addr,sizeof(struct sockaddr));
    s_addr->sin_family = AF_INET;
    s_addr->sin_port = htons(port);
    err = inet_aton(p_ip_addr, &s_addr->sin_addr);
    if( err == 0)
    {
        printf("Server IP Address Error!\n");
        return -1;
    }  
    
    *sock = cli_sock;
    return 0;    
}


/**@brief             从服务器下载升级文件 
 * @param sock        套接字描述符
 * @param file_name   文件名
 * @param file_buffer 文件缓存，存放升级文件
 * @param finish_len  已传送文件大小
 * @return            0:正常，-1：异常
 */ 
int client_recv_file(int sock, char *file_name, char **file_buffer, unsigned int *finish_len)
{	
    int err = 0;                     
    int length = 0;	 
    int percent = 0; 
    int write_length = 0;    
    unsigned int sum = 0;
    unsigned int rmnd = 0;    
    unsigned int file_size = 0;   // 升级文件大小    
    char buffer[TCP_BUF_SIZE];		
    
    // 组首发包（finish_len + file_name）
    bzero(buffer,TCP_BUF_SIZE);
    *((unsigned int *)buffer) = *finish_len;
    memcpy(buffer + sizeof(int), file_name, strlen(file_name));
    
    // 发送首包（finish_len + file_name）    
    err = send(sock, buffer, TCP_BUF_SIZE, 0); 
    if(err < 0)
    {
    	printf("send first packet error\n");
		close(sock);
    	return -1;
    } 
    
    // 接收首包应答
    err = recv(sock, buffer, FILE_NAME_SIZE, 0);      
    if(err < 0)
    {
    	printf("send first packet error\n");
		close(sock);
    	return -1;
    }
	
	// 接收升级文件大小
	bzero(buffer,TCP_BUF_SIZE);
    err = recv(sock, buffer, FILE_NAME_SIZE, 0);
    if(err < 0)
    {            
            printf("\n\t\trecv error!\n");
			close(sock);
            return -1;
    }  
    
    file_size = (unsigned int)atoi(buffer + 1);      
    rmnd = file_size / 100; 
    
    switch(buffer[0])
    {
    	case 'N': printf("\n\t\t服务器上无升级文件！\n");
    	          exit(0);
    	          break;
    	          
    	case 'Y': // 初次接收文件，需要分配内存    			  
    	          *file_buffer = (char *)malloc(file_size);
    			  if(*file_buffer == NULL)
    			  {
    			      printf("\n\t\tmalloc error\n");
					  close(sock);
    			      exit(0);
    			  }    			  
    			  break;
    			  
    	case 'G': // 续传文件,不用分配内存
    	          break;
    	          
    	default:  //意外数据
    	          printf("\n\t\t接收升级文件大小时，数据格式错误！\n");
				  close(sock);
				  exit(-1);
    }       
    
    printf("\n\t\t升级文件 %s 接收中:",file_name);
    printf("%%%2d",*finish_len / rmnd); 
    
    bzero(buffer,TCP_BUF_SIZE); 
    isRecv = 1;  
    while(*finish_len < file_size)
    {            
        if(isRecv == 0)
        {
        	printf("\n\t\t服务器丢失，即将进行重新连接...\n");
			close(sock);
        	return -1;
        }
        
        length = recv(sock, buffer, TCP_BUF_SIZE , 0);
        if(length == 0)
        {
        	printf("\n\t\t服务器关闭，即将进行重新连接...\n");
			close(sock);
        	break;
        }
        else if(length < 0 )
        {
        	if(errno == EINTR || errno == ECONNRESET)
        	{
        		continue;        		
        	}
        	else
        	{    
        	    printf("\nrecv error, errno = %d\n", errno);
				close(sock);        	    
    			break;
        	}     	
        }        
         
        memcpy((*file_buffer) + (*finish_len), buffer, length);  
        (*finish_len) += length;     
        
        //进度显示        
        if((*finish_len / rmnd) != percent)
        {
            percent = *finish_len / rmnd;
            printf("\b\b\b%%%2d", percent);        
            fflush(stdout); 
        }                     
        
        bzero(buffer,TCP_BUF_SIZE);
    }  
    
    if( *finish_len < file_size)
    {
        printf("\n\t\t接收未完成，进行【续传】！\n");    	
    	close(sock);            //关套接字流
    	return -1;
    }    
         
    printf("\n\n\t\t文件:  %s 接收完毕!\n",file_name);       
	return 0;	
}
