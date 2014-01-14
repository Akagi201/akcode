/**@file  server_send_file.c
 * @note  HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief 服务器中的发送升级包的各个函数定义 
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

#include "server_send_file.h"
                    
                      
/**@brief          忽略子进程退出信号
 * @return         无
 */                    
void sig_sigpipe(int signo)
{}



/**@brief          初始化套接字，并监听
 * @param port     端口号
 * @param p_sock   套接字回传
 * @return         0:right, -1:error
 */
int server_sock_init(int port, int *p_sock)
{
	struct sockaddr_in ser_addr;
	int sock;
	int on = 1;
	
	bzero(&ser_addr,sizeof(ser_addr));            //内存&server_addr 空间清零
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = htons(INADDR_ANY);
    ser_addr.sin_port = htons(port);
    
    //创建用于internet的流协议（TCP）socket
    sock = socket(PF_INET,SOCK_STREAM,0);  
    if( sock < 0)
    {
        printf("Create tcp Socket Failed!");
        exit(1);
    }
    
     //使能端口重用
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );  
    
    //绑定，socket和socket地址结构     
    if( bind(sock,(struct sockaddr*)&ser_addr,sizeof(ser_addr)))   
    {
        printf("Server Bind Port : %d Failed!", port);
        exit(1);
    }

    if ( listen(sock, TCP_LISTEN_QUEUE) )     // 监听队列：20
    {
        printf("Server Listen Failed!");
		close(sock);
        exit(1);
    }
    
    *p_sock = sock;
    return 0;
}


/**@brief          向客户端发送文件 
 * @param sock     套接字描述符
 * @param cli_ip   客户端IP 
 * @param cli_port 客户端端口号
 * @return         0:正常，-1：异常
 */ 
int server_send_file(int sock, char *cli_ip, int cli_port)
{	
    int i = 0;
	int sum = 0;	
	int send_len = 0; 
	unsigned int file_size = 0;
	unsigned int finish_len = 0;
	char buffer[TCP_BUF_SIZE];
	char file_name[FILE_NAME_SIZE];
	FILE * fp = NULL;
	
	assert(sock >= 0);
	assert(cli_ip != NULL);
	
	bzero(buffer, TCP_BUF_SIZE);
	
	
	// 接收：已传长度 + 文件名
    if(recv(sock, buffer, TCP_BUF_SIZE, 0) < 0)
    {
    	printf("recv file name error\n");
    	close(sock);
    	return -1;
    }
    
	// 确认文件名接收成功   
    if(send(sock, buffer, FILE_NAME_SIZE,0) < 0)
    {
    	printf("send file name ack error\n");
    	close(sock);
    	return -1;
    }
    
    // 读取已传文件大小 + 文件名
    finish_len = *((unsigned int *)buffer); 
    strcpy(file_name, buffer + sizeof(int));    
    
    // 准备传输升级文件
    fp = fopen(file_name,"r");
    if(NULL == fp )
    {
	    // 无此文件
        printf("File:\t%s Not Found\n", file_name);
        if(send(sock, "N", FILE_NAME_SIZE,0) < 0)
        {
        	printf("\t\tserver: send N error\n");
        }
        
        close(sock);
        return -1;
    }
    else
    {   
        // 读取文件大小 
        fseek(fp, 0, SEEK_END);
        file_size = ftell(fp);               
        
        bzero(buffer, TCP_BUF_SIZE);
        
        if(finish_len == 0)
        {
            // 传送状态:始发
        	buffer[0] = 'Y';
        }
        else
        {
        	// 传送状态:续传
        	buffer[0] = 'G';
        }        
        
        // 发送 传送状态 + 文件大小        
        sprintf(buffer + 1, "%d", file_size);
        send(sock, buffer, FILE_NAME_SIZE, 0); 
        usleep(500);
        
        printf("\n\t\t[%s:%d]:正在发送升级文件<%s>...", cli_ip, cli_port, file_name);  
        fflush(stdout);
        
        // 定位文件开始传送位置           
		fseek(fp, finish_len, SEEK_SET);
        while(!feof(fp))
        {    
            // 读取数据		
            send_len = fread(buffer, sizeof(char), TCP_BUF_SIZE , fp);         
            if(send_len < 0)
            {
            	printf("\nfread error\n");
            	return -1;            	
            }
			
			// 发送数据
            if(send(sock, buffer, send_len, 0) < 0)
            {
                printf("\n\n\t\t网络连接已断开，中止发送文件:%s\n", file_name);
                sleep(20);
                close(sock);
                fclose(fp);
                return -1;
            }            
            
			// 延时，降低传送速度，方便观察
            usleep(1000);        	            
            bzero(buffer, TCP_BUF_SIZE);
        } 
		
        printf("\n\t\t[%s:%d]:升级文件[%s] 发送完毕！\n", cli_ip, cli_port, file_name); 
        printf("\n\t\t[%s:%d]:设备升级中...", cli_ip, cli_port);
        fflush(stdout);
        
		// 设置超时，等待客户端回传升级进度
        struct timeval rcvTout;
        rcvTout.tv_sec = 6;
        rcvTout.tv_usec = 0;
        if( setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,&rcvTout, sizeof(rcvTout)) < 0) 
        {
        	printf("setsockopt error\n");
        	exit(-1);
        }        
        
		// 对升级文件进行编号
        i = 1;
		
        while(1)
        {
        	bzero(buffer, FILE_NAME_SIZE);
        	if(recv(sock, buffer, FILE_NAME_SIZE, 0) < 0)
        	{
        		if(errno == EINTR)
        		{
        			continue;
        		}
        		else
        		{
        			printf("\n\t\t与[%s:%d]连接断开，升级失败！\n", cli_ip, cli_port);
        			exit(-1);
        		}
        	}
        	else
        	{        		
        		if(buffer[0] == 'R')
        		{	
        		    // 升级结果
        			printf("\n\t\t[%s:%d]:%s\n", cli_ip, cli_port, buffer + 1);
        			break;
        		}        		
        		else
        		{
        			// 升级进度
        			printf("\n\t\t[%s:%d]:<%2d>%s\n", cli_ip, cli_port, i++, buffer);        			
        		}
        	}
        }      
    } 
    heartbeat_serv(sock, 0, 2);  
    fclose(fp);  
    close(sock);     
	return 0;	
}
