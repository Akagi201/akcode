/**@file  client.c
 * @note  HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief 客户端，设备网络升级程序 
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
#include "client_check_file.h"


/**@brief         客户端主函数，控制调用流程
 * @param args    参数个数
 * @param argv[1] 服务器IP地址
 * @param argv[2] 端口
 * @return        0:正常，-1：异常
 */ 
int main(int args, char **argv)    /** 客户端 **/
{   
	int i = 0; 
    int sock = 0;	
	int err = 0;
	unsigned int finish_len = 0;
	char *file_buffer = NULL;
	char file_name[FILE_NAME_SIZE] = "digicap.dav";		
	struct sockaddr_in ser_addr;  
    
    finish_len = 0;
    while(1)
    {
    	err = client_sock_init( argv[1], atoi(argv[2]), &sock, &ser_addr);
		if(err < 0)
		{
			printf("sock_init error\n");
			return -1;
		}
		
    	// 连接服务器
    	err = connect(sock,(struct sockaddr*)&ser_addr, sizeof(ser_addr));
    	if( err < 0)
    	{
    		alarm(0);
        	printf("\n\t\tCan Not Connect To %s! \n\t\t15秒后重试...15",argv[1]);
        	
        	for(i = 0; i < 15; i++)
        	{
        		sleep(1);
        		printf("\b\b%2d", 14 - i);
        		fflush(stdout);
        	}
        	printf("\n");        	
        	continue;
    	}    	
    	
    	heartbeat_cli(sock, 10);
    	err = client_recv_file(sock, file_name, &file_buffer, &finish_len);    	
    	if(err < 0)
    	{
    		// 未完成接收，续传   		
    		continue;
    	}
    	else
    	{   // 已完成接收
    		break;
    	}
    } 
    
    //检测并写入       
    check_file(sock , file_buffer);
    
    free(file_buffer);
    file_buffer = NULL;
    
    return 0;
} 










