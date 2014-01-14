/**@file  client_recv_file.c
 * @note  HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief �ͻ����е������ļ����յĸ����������� 
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
                      

/**@brief          ��ʼ���׽���
 * @param port     �˿ں�
 * @param p_sock   �׽��ֻش�
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


/**@brief             �ӷ��������������ļ� 
 * @param sock        �׽���������
 * @param file_name   �ļ���
 * @param file_buffer �ļ����棬��������ļ�
 * @param finish_len  �Ѵ����ļ���С
 * @return            0:������-1���쳣
 */ 
int client_recv_file(int sock, char *file_name, char **file_buffer, unsigned int *finish_len)
{	
    int err = 0;                     
    int length = 0;	 
    int percent = 0; 
    int write_length = 0;    
    unsigned int sum = 0;
    unsigned int rmnd = 0;    
    unsigned int file_size = 0;   // �����ļ���С    
    char buffer[TCP_BUF_SIZE];		
    
    // ���׷�����finish_len + file_name��
    bzero(buffer,TCP_BUF_SIZE);
    *((unsigned int *)buffer) = *finish_len;
    memcpy(buffer + sizeof(int), file_name, strlen(file_name));
    
    // �����װ���finish_len + file_name��    
    err = send(sock, buffer, TCP_BUF_SIZE, 0); 
    if(err < 0)
    {
    	printf("send first packet error\n");
		close(sock);
    	return -1;
    } 
    
    // �����װ�Ӧ��
    err = recv(sock, buffer, FILE_NAME_SIZE, 0);      
    if(err < 0)
    {
    	printf("send first packet error\n");
		close(sock);
    	return -1;
    }
	
	// ���������ļ���С
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
    	case 'N': printf("\n\t\t���������������ļ���\n");
    	          exit(0);
    	          break;
    	          
    	case 'Y': // ���ν����ļ�����Ҫ�����ڴ�    			  
    	          *file_buffer = (char *)malloc(file_size);
    			  if(*file_buffer == NULL)
    			  {
    			      printf("\n\t\tmalloc error\n");
					  close(sock);
    			      exit(0);
    			  }    			  
    			  break;
    			  
    	case 'G': // �����ļ�,���÷����ڴ�
    	          break;
    	          
    	default:  //��������
    	          printf("\n\t\t���������ļ���Сʱ�����ݸ�ʽ����\n");
				  close(sock);
				  exit(-1);
    }       
    
    printf("\n\t\t�����ļ� %s ������:",file_name);
    printf("%%%2d",*finish_len / rmnd); 
    
    bzero(buffer,TCP_BUF_SIZE); 
    isRecv = 1;  
    while(*finish_len < file_size)
    {            
        if(isRecv == 0)
        {
        	printf("\n\t\t��������ʧ������������������...\n");
			close(sock);
        	return -1;
        }
        
        length = recv(sock, buffer, TCP_BUF_SIZE , 0);
        if(length == 0)
        {
        	printf("\n\t\t�������رգ�����������������...\n");
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
        
        //������ʾ        
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
        printf("\n\t\t����δ��ɣ����С���������\n");    	
    	close(sock);            //���׽�����
    	return -1;
    }    
         
    printf("\n\n\t\t�ļ�:  %s �������!\n",file_name);       
	return 0;	
}
