/**@file  server.c
 * @note  HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief ���������豸������������ 
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
#include "server_heart.h"


/**@brief         �����������Ƶ�������
 * @param args    ��������
 * @param argv[1] �˿�
 * @return        0:������-1���쳣
 */       
int main(int args, char **argv)   
{   	
	int sock = 0;
	int con_sock = 0;	
	int err = 0;
	int clilen = 0;
	struct sockaddr_in cli_addr;
	
	signal(SIGPIPE, sig_sigpipe);
	err = server_sock_init(atoi(argv[1]), &sock);
	if(err < 0)
	{
		printf("sock_init error\n");
		return -1;
	}
		
	signal(SIGCHLD, sig_chld);	

    //�������̣߳�����������������	
	while(1)       
    {         
        con_sock = accept(sock, (struct sockaddr*)&cli_addr,&clilen);  //�ȴ�TCP����
        if(con_sock < 0)
        {
           printf("TCP Server Accept Failed!\n");
            continue;
        }          
        
        if(fork() == 0)
        {   //�ӽ��̹���
		
        	close(sock);        	
        	heartbeat_serv(con_sock, 5, 2); 
        	
        	// start send updata_file to client        	
        	server_send_file(con_sock, inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port)); 
        	exit(0);                   
        }        
        close(con_sock);
    }
    return 0;
} 
