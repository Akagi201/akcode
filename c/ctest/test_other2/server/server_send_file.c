/**@file  server_send_file.c
 * @note  HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief �������еķ����������ĸ����������� 
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
                    
                      
/**@brief          �����ӽ����˳��ź�
 * @return         ��
 */                    
void sig_sigpipe(int signo)
{}



/**@brief          ��ʼ���׽��֣�������
 * @param port     �˿ں�
 * @param p_sock   �׽��ֻش�
 * @return         0:right, -1:error
 */
int server_sock_init(int port, int *p_sock)
{
	struct sockaddr_in ser_addr;
	int sock;
	int on = 1;
	
	bzero(&ser_addr,sizeof(ser_addr));            //�ڴ�&server_addr �ռ�����
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = htons(INADDR_ANY);
    ser_addr.sin_port = htons(port);
    
    //��������internet����Э�飨TCP��socket
    sock = socket(PF_INET,SOCK_STREAM,0);  
    if( sock < 0)
    {
        printf("Create tcp Socket Failed!");
        exit(1);
    }
    
     //ʹ�ܶ˿�����
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );  
    
    //�󶨣�socket��socket��ַ�ṹ     
    if( bind(sock,(struct sockaddr*)&ser_addr,sizeof(ser_addr)))   
    {
        printf("Server Bind Port : %d Failed!", port);
        exit(1);
    }

    if ( listen(sock, TCP_LISTEN_QUEUE) )     // �������У�20
    {
        printf("Server Listen Failed!");
		close(sock);
        exit(1);
    }
    
    *p_sock = sock;
    return 0;
}


/**@brief          ��ͻ��˷����ļ� 
 * @param sock     �׽���������
 * @param cli_ip   �ͻ���IP 
 * @param cli_port �ͻ��˶˿ں�
 * @return         0:������-1���쳣
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
	
	
	// ���գ��Ѵ����� + �ļ���
    if(recv(sock, buffer, TCP_BUF_SIZE, 0) < 0)
    {
    	printf("recv file name error\n");
    	close(sock);
    	return -1;
    }
    
	// ȷ���ļ������ճɹ�   
    if(send(sock, buffer, FILE_NAME_SIZE,0) < 0)
    {
    	printf("send file name ack error\n");
    	close(sock);
    	return -1;
    }
    
    // ��ȡ�Ѵ��ļ���С + �ļ���
    finish_len = *((unsigned int *)buffer); 
    strcpy(file_name, buffer + sizeof(int));    
    
    // ׼�����������ļ�
    fp = fopen(file_name,"r");
    if(NULL == fp )
    {
	    // �޴��ļ�
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
        // ��ȡ�ļ���С 
        fseek(fp, 0, SEEK_END);
        file_size = ftell(fp);               
        
        bzero(buffer, TCP_BUF_SIZE);
        
        if(finish_len == 0)
        {
            // ����״̬:ʼ��
        	buffer[0] = 'Y';
        }
        else
        {
        	// ����״̬:����
        	buffer[0] = 'G';
        }        
        
        // ���� ����״̬ + �ļ���С        
        sprintf(buffer + 1, "%d", file_size);
        send(sock, buffer, FILE_NAME_SIZE, 0); 
        usleep(500);
        
        printf("\n\t\t[%s:%d]:���ڷ��������ļ�<%s>...", cli_ip, cli_port, file_name);  
        fflush(stdout);
        
        // ��λ�ļ���ʼ����λ��           
		fseek(fp, finish_len, SEEK_SET);
        while(!feof(fp))
        {    
            // ��ȡ����		
            send_len = fread(buffer, sizeof(char), TCP_BUF_SIZE , fp);         
            if(send_len < 0)
            {
            	printf("\nfread error\n");
            	return -1;            	
            }
			
			// ��������
            if(send(sock, buffer, send_len, 0) < 0)
            {
                printf("\n\n\t\t���������ѶϿ�����ֹ�����ļ�:%s\n", file_name);
                sleep(20);
                close(sock);
                fclose(fp);
                return -1;
            }            
            
			// ��ʱ�����ʹ����ٶȣ�����۲�
            usleep(1000);        	            
            bzero(buffer, TCP_BUF_SIZE);
        } 
		
        printf("\n\t\t[%s:%d]:�����ļ�[%s] ������ϣ�\n", cli_ip, cli_port, file_name); 
        printf("\n\t\t[%s:%d]:�豸������...", cli_ip, cli_port);
        fflush(stdout);
        
		// ���ó�ʱ���ȴ��ͻ��˻ش���������
        struct timeval rcvTout;
        rcvTout.tv_sec = 6;
        rcvTout.tv_usec = 0;
        if( setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,&rcvTout, sizeof(rcvTout)) < 0) 
        {
        	printf("setsockopt error\n");
        	exit(-1);
        }        
        
		// �������ļ����б��
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
        			printf("\n\t\t��[%s:%d]���ӶϿ�������ʧ�ܣ�\n", cli_ip, cli_port);
        			exit(-1);
        		}
        	}
        	else
        	{        		
        		if(buffer[0] == 'R')
        		{	
        		    // �������
        			printf("\n\t\t[%s:%d]:%s\n", cli_ip, cli_port, buffer + 1);
        			break;
        		}        		
        		else
        		{
        			// ��������
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
