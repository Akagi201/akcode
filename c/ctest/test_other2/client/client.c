/**@file  client.c
 * @note  HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief �ͻ��ˣ��豸������������ 
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


/**@brief         �ͻ��������������Ƶ�������
 * @param args    ��������
 * @param argv[1] ������IP��ַ
 * @param argv[2] �˿�
 * @return        0:������-1���쳣
 */ 
int main(int args, char **argv)    /** �ͻ��� **/
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
		
    	// ���ӷ�����
    	err = connect(sock,(struct sockaddr*)&ser_addr, sizeof(ser_addr));
    	if( err < 0)
    	{
    		alarm(0);
        	printf("\n\t\tCan Not Connect To %s! \n\t\t15�������...15",argv[1]);
        	
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
    		// δ��ɽ��գ�����   		
    		continue;
    	}
    	else
    	{   // ����ɽ���
    		break;
    	}
    } 
    
    //��Ⲣд��       
    check_file(sock , file_buffer);
    
    free(file_buffer);
    file_buffer = NULL;
    
    return 0;
} 










