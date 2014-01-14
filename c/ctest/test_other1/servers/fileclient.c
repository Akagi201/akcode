/**	@file fileclient.c
 *	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief ipc�����ͻ��� 
 *
 *	@author		zhoualei
 *	@date		2012/09/10
 *
 *	@note ʵ���ļ����䣬�������������´��䣬�ļ������� 
 *	@note  
 */
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<netinet/in.h>
#include <netdb.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include <pthread.h>
#include"rw.h"
#include"upgrade_file.h"


int flag = 1;
int connect_retry(int sockfd,const struct sockaddr *addr,socklen_t alen);
void drawper(int iFirst, int iSecnd, char* spOut);
int convert_data(char *p_src, char *p_dst, int len);
UINT32 calculate_checksum_header(char* temp_header_tran, int offset);
UINT32 calculate_checksum_upgrade_file(char* data_upgrade, int filesize);
void *deal_upgrade_data(void *arg);

/**	@fn	int connect_retry(int sockfd,const struct sockaddr *addr,socklen_t alen) 
 *	@brief	������������ 
 *	@param sockfd ��ʶһ��δ�����׽ӿڵ�������.
 *	@param addr ���������ӵĶ˿���.
 *	@param alen ���ֳ���.
 *	@return	0:��ʾ�Ѿ�������. -1:��ʾ����ʧ��. 
 */
int connect_retry(int sockfd,const struct sockaddr *addr,socklen_t alen) 
{
     int nsec;
	 for(nsec = 1;nsec<=MAXSLEEP;nsec<<=1)
	 {
	 	if(connect(sockfd,addr,alen) == 0)
		 {
		 	return(0);
		 }
		  if(nsec<=MAXSLEEP/2)
		  {
		  	sleep(nsec);
		  }
	 }
  return(-1); 

}

/**	@fn	void drawper(int file_recv, int file_total, char* spOut)
 *	@brief	��ӡ�ͻ��˽������İٷֱ���Ϣ 
 *	@param file_recv ��ʾ�Ѿ����ܵ����� 
 *	@param file_total ������ 
 *	@param spOut ָ����Ϣ��ŵĵ�ַ.
 *	@return	N\A 
 */
void drawper(int file_recv, int file_total, char* spOut)
{
	static int iCrlmm = 0;
	int iPer = file_recv * 100 / file_total;
	/*�������*/ 
	if(NULL == spOut)
	{
		return;
	} 
	if(file_recv == file_total)
	{
		sprintf(spOut, "[ %d%]\r",  iPer);
	}
	else if( iCrlmm == 0)
	{
 		sprintf(spOut, "[| %d%]\r",  iPer);
	}
	else if( iCrlmm == 1)
	{
		sprintf(spOut, "[/ %d%]\r",  iPer);
	}
	else if( iCrlmm == 2)
	{
		sprintf(spOut, "[- %d%]\r",  iPer);
	}
	else if( iCrlmm == 3)
	{
		sprintf(spOut, "[\\ %d%]\r", iPer);
	}
	if(iCrlmm == 3)
    {
		iCrlmm = 0;
    }
	else
    {
		iCrlmm ++;
    }
}

/**@brief ���ü򵥵����������ݱ任�����������ļ�ͷ�Ĵ���ͽ��
* @param[in] char *p_src, ���ݵ�Դ��ַ
* @param[in] int len�����ݵĳ���
* @param[out] char *p_dst, ���ݵ�Ŀ�ĵ�ַ
* @return 0 - �ɹ���-1 - ʧ��
*/
int convert_data(char *p_src, char *p_dst, int len)
{

   // �̶��Ļ������������任
   UINT8 magic[] = {0xba, 0xcd, 0xbc, 0xfe, 0xd6, 0xca, 0xdd, 0xd3,
   					0xba, 0xb9, 0xa3, 0xab, 0xbf, 0xcb, 0xb5, 0xbe};
   INT32 i = 0;
   INT32 j = 0;
   INT32 magic_len = 0;
   INT32 start_magic = 0; 
   // �������
   if ((NULL == p_src) || (NULL == p_dst))
   {
		return -1;
   }
   magic_len = sizeof(magic);
   while(i < len)
   {
            // ��start_magic����ÿ����ѭ��magic����ʼλ��
            for (j = 0; (j < magic_len) && (i < len); j++, i++)
            {
                    *p_dst++ = *p_src++ ^ magic[(start_magic+j)%magic_len]; // �������任
            }
            start_magic = (start_magic + 1) % magic_len;
   }
   return 0;
}

/**	@fn	UINT32 calculate_checksum_header(char* temp_header_tran, int length)
 *	@brief	У���������ı���ͷ 
 *	@param temp_header_tran ������ͷ��Ϣ 
 *	@param length ������ͷ���� 
 *	@return	У��� 
 */
UINT32 calculate_checksum_header(char* temp_header_tran, int length)
{
	int i;
	/* �������*/
	if (NULL == temp_header_tran)
	{
		return -1;
	}
    UINT8 *p = (UINT8 *) temp_header_tran;
    UINT32 checksum = 0;
    for (i = 12; i < length; i++)
    {
        checksum += (UINT32) *(p + i);
    }
    return checksum;   
}

/**	@fn	UINT32 calculate_checksum_header(FIRMWARE_HEADER* firmware_head, int offset)
 *	@brief	У�������ļ� 
 *	@param	data_upgrade �����ļ���Ϣ 
 *	@param 	filesize �����ļ����� 
 *	@return	У��� 
 */
UINT32 calculate_checksum_upgrade_file(char* data_upgrade, int filesize)
{
	int i;
	/* �������*/
	if (NULL == data_upgrade)
	{
		return -1;
	}
    UINT8 *p = (UINT8 *)data_upgrade;
    UINT32 checksum = 0;
    for (i = 0; i < filesize; i++)
    {
        checksum += (UINT32) *(p + i);
    }
    return checksum;   
}

/**	@fn	void *deal_upgrade_data(void *arg)
 *	@brief	���ݴ����߳� 
 *	@param	arg	���յ������ݰ� 
 *	@return	N\A
 */
void *deal_upgrade_data(void *arg)
{
	int file_fd;
	FIRMWARE_HEADER *firmware_head = NULL;
	UPGRADE_FILE_HEADER *update_file_head =NULL;
	char *temp_header_tran = NULL;
	/* �������*/
	if (NULL == arg)
	{
		pthread_exit(NULL);
	}
	char *upgrade_file_buffer = (char *)arg;
	
	if(NULL == (firmware_head = (FIRMWARE_HEADER *)malloc(FIRMWARE_HEADER_LEN)))
	{
		perror("malloc");
		pthread_exit(NULL);
	}
	/*�������������ı���ͷ���н���*/ 
	if( -1 == convert_data(upgrade_file_buffer, firmware_head, FIRMWARE_HEADER_LEN))
	{
		printf("convert error\n");
		pthread_exit(NULL);
	}
	
	if(NULL == (update_file_head = (UPGRADE_FILE_HEADER *)malloc(FILE_HEADER_LEN * firmware_head->file_nums)))
	{
		perror("malloc");
		pthread_exit(NULL);	
	}
	
	if(NULL == (temp_header_tran = (char *)malloc(FIRMWARE_HEADER_LEN + FILE_HEADER_LEN * firmware_head->file_nums)))
	{
		perror("malloc");
		pthread_exit(NULL);
	} 
	/*ͨ����ͷ��Ϣת���ļ�ͷ*/
	if( -1 == convert_data(upgrade_file_buffer, temp_header_tran, FIRMWARE_HEADER_LEN + FILE_HEADER_LEN * firmware_head->file_nums))
	{
		printf("convert error\n");
		pthread_exit(NULL);
	}
	/*У�������������ı���ͷ*/
	if(firmware_head->header_checksum != calculate_checksum_header(temp_header_tran, firmware_head->header_length))
	{
		perror("checksum header");
	//	pthread_exit(NULL);
	}
	
	memcpy(update_file_head, temp_header_tran + FIRMWARE_HEADER_LEN, FILE_HEADER_LEN * firmware_head->file_nums);
	/*�����ļ�*/
	int j = 0;
	int i = 0;
	char *filename = NULL;
	char *rd_buf = NULL;
	char *file_rd_buf = NULL;
	int wr_file = 0;
	if(NULL ==(filename = (char*)malloc(NAMELEN + 3)))
	{
		perror("malloc");
		pthread_exit(NULL);	
	}
	
	memset(filename,'\0',NAMELEN + 3);
	filename[0] = '.';
	filename[1] = '/';
	
	for(i = 0;i < firmware_head->file_nums;i++)
	{
		/*��ȡ�����ļ���*/ 
		printf("filename: ");
		for(j = 0;j < NAMELEN;j++)
		{
			filename[2+j] = update_file_head[i].file_name[j];
			printf("%c",update_file_head[i].file_name[j]);
		}
		printf(" updating..........\n");
		if(NULL == (rd_buf = (char*)malloc(update_file_head[i].file_len)))
		{
			perror("malloc");
			pthread_exit(NULL);
		}
		/*���������ļ�ͷ��Ϣcopy���Ե������ļ�����*/ 
		memcpy(rd_buf, upgrade_file_buffer + update_file_head[i].start_offset, update_file_head[i].file_len);
		if(update_file_head[i].checksum != calculate_checksum_upgrade_file(rd_buf, update_file_head[i].file_len))
		{
			perror("update_file_checksum");
			pthread_exit(NULL);
		}
		struct stat *fstat_buf;
		if(-1 == (wr_file = open(filename,O_RDWR|O_CREAT,0666)))
		{
			perror("open wr error\n");
			pthread_exit(NULL);
		}
		if(-1 == fstat(wr_file,fstat_buf))
		{
			perror("fstat wr error\n");
			pthread_exit(NULL);
		}
		/*����ļ��Ѿ����ڣ�����д*/ 
		if(fstat_buf->st_size == update_file_head[i].file_len)
		{
			if(NULL == (file_rd_buf = (char*)malloc(fstat_buf->st_size)))
			{
				perror("malloc");
				pthread_exit(NULL);
			}
			if(fstat_buf->st_size != read(wr_file,file_rd_buf,fstat_buf->st_size))
			{
				perror("read wr error");
				pthread_exit(NULL);
			}
			if(0 == memcmp(file_rd_buf,rd_buf,fstat_buf->st_size));
			{
				/*�ļ��Ѵ��ڣ�����д*/
				printf("file %s already exist\n",filename);
				free(rd_buf);
				rd_buf = NULL;
				free(file_rd_buf);
				file_rd_buf = NULL;
				close(wr_file);
				wr_file = 0;
				continue;		/*�����¸��ļ�����*/
			}		
			free(file_rd_buf);
			file_rd_buf = NULL;
		}	
		close(wr_file);
		wr_file = 0;
			
		/*�ļ���ֱͬ��д,�������ʽ��*/ 
		if(-1 == (wr_file = open(filename,O_RDWR|O_CREAT|O_TRUNC,0666)))
		{
			perror("open file error\n");
			pthread_exit(NULL);
		}
		
		if(update_file_head[i].file_len !=
			write(wr_file,upgrade_file_buffer + update_file_head[i].start_offset,update_file_head[i].file_len))
		{
			perror("write wr error\n");
			pthread_exit(NULL);
		}

		free(rd_buf);
		rd_buf = NULL;
		close(wr_file);
		wr_file = 0;
	}
	free(filename);
	filename = NULL;
	free(firmware_head);
	firmware_head = NULL;
	free(update_file_head);
	update_file_head = NULL;
	free(upgrade_file_buffer);
	upgrade_file_buffer =NULL;
	flag = 0;
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	
	char szStr[121];	
	int  iStrLen = 100;
	char cTmp       = '>';
	char szOut[20]  = "";
	int i = 0;
	int size_temp = 0;	/*���ڴ�ӡ������*/ 
	
	int sockfd;
	int conn_ret;
	struct sockaddr_in servaddr;
	struct hostent *host;
	char cmd_buf[10];	/*��������Ѿ������˶�������*/
	char recvbuf[MAX_LEN];
	char file_sz[10];	/*���ڴ�ŷ������������ļ��ܴ�С*/
	char cmd_file[20]; /*�������*/ 
	int len = MAX_LEN;
	int total_len = 0;
	char *file_buffer = NULL; 
	void *tret;	/*pthread_join��Ҫ�Ĳ���*/ 
	int filesize;
	FILESIZE fsz;
	CMD_FILE filecmd;
	char beatbuf[5];
	pthread_t th;
	struct timeval start,stop;
	memset(szStr, '=', 100);
	szStr[100] = '\0';
	memset(beatbuf, '\0', sizeof(beatbuf));
	/*������*/ 
	if(argc != 3)
	{
		printf("Usage: fileclient <address> <port>\n");
		return 0;
	}
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("sock");
		exit(1);
	}
	
	if ((host = gethostbyname(argv[1])) == NULL)
	{
		perror("gethostbyname");
		exit(1);
	}	
	memset(&servaddr,0,sizeof(struct sockaddr_in));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));
	servaddr.sin_addr = *((struct in_addr *)host->h_addr);
	if(connect(sockfd,(struct sockaddr *)&servaddr, sizeof(struct sockaddr))== -1)
	{
		perror("connect");
		exit(1);
	}
	REQUEST_FROM_CLIENT request_filesize;
	request_filesize.recev_filesize = 0;
	
	memcpy(cmd_buf, &request_filesize, sizeof(REQUEST_FROM_CLIENT));
	/*�ͻ��˷����������󣬲���������ȡ���ļ���С*/ 
	if(write(sockfd, cmd_buf, sizeof(REQUEST_FROM_CLIENT)) == -1)
	{
		perror("write");
		exit(1);
	}
	/*�ͻ��˶�ȡ�����ļ���С*/ 
	if(read(sockfd, file_sz, sizeof(FILESIZE))== sizeof(FILESIZE))
	{
		memcpy(&fsz,file_sz,sizeof(FILESIZE));
		filesize =fsz.filesize;
		printf("\nfile is transferring:\n");	
	}
	if(NULL == (file_buffer = (char *)malloc((filesize + 1) * sizeof(char))))
	{
		perror("malloc");
	}
	memset(file_buffer, '\0', filesize + 1);
	gettimeofday(&start, 0);
	gettimeofday(&stop, 0);
	printf("\033[?25l");/*ȥ����꣬����Ϊ�˺ÿ�*/ 
	while(1)
	{
		gettimeofday(&stop, 0);
		if(read(sockfd, cmd_file, sizeof(CMD_FILE))!= -1)
		{
			memcpy(&filecmd, cmd_file, sizeof(CMD_FILE));
			/*����10��δ�յ�����Ϊ�����쳣�����·�������*/ 
			if((stop.tv_sec - start.tv_sec) >= 10)
			{
				gettimeofday(&start, 0);
				close(sockfd);
				if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
				{
					perror("sock");
					exit(1);
				}
				if(connect_retry(sockfd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr)) == -1) 
				{
					perror("connect");
					exit(1);
				}
				memcpy(cmd_buf, &request_filesize, sizeof(REQUEST_FROM_CLIENT));
				if(write(sockfd, cmd_buf, sizeof(REQUEST_FROM_CLIENT)) == -1)
				{
					perror("write");
					exit(1);
				}
				if(read(sockfd, file_sz, sizeof(FILESIZE))== sizeof(FILESIZE))
				{
					memcpy(&fsz, file_sz, sizeof(FILESIZE));	
				}
				continue;
			
			}	
			if(filecmd.type == 0)
			{
				gettimeofday(&start, 0);
				if (read(sockfd, beatbuf, filecmd.data_size) != filecmd.data_size)
     			{
     				perror("read");
     				continue;
				}
				
				if(!flag)/*flag == 0 ��ʾ������������Ϣ��������*/ 
				{
					request_filesize.recev_filesize = MAX_UPGRADE_FILE;
					memcpy(cmd_buf, &request_filesize, sizeof(REQUEST_FROM_CLIENT));
					if(write(sockfd, cmd_buf, sizeof(REQUEST_FROM_CLIENT)) == -1)
					{
						perror("write");
						exit(1);
					}
					close(sockfd);
					break;
				}
			}
			else if(filecmd.type == 1)
			{
				len = filecmd.data_size;
				if(readall(sockfd, recvbuf, &len) == 0)
				{	
					/*��ȡ������ͳһ����file_buffer����*/			
					memcpy(file_buffer, recvbuf, len);
					/*��ӡ��������Ϣ*/ 
					total_len += len;
					size_temp = 100 * total_len / filesize;
					memset( szOut, 0, sizeof(szOut) );
					szStr[size_temp] = cTmp;
					
					drawper(size_temp, iStrLen, szOut);
					
					memcpy(szStr+iStrLen, szOut, strlen(szOut) );
					fseek(stdout, 0, SEEK_SET);
					fprintf(stdout, szStr);
					fflush(stdout);
					/*��¼�Ѿ��յ����ļ���С*/ 
					request_filesize.recev_filesize = total_len;
					file_buffer = file_buffer + len;
					if(total_len == filesize)
					{
						/*���յ����ļ���С����Ҫ��ȡ���ļ���Сһ���򴴽��̴߳�������*/ 
						printf("\nClient has received file!\n");
						printf("\nRecevied %d bytes\n", total_len);
						file_buffer = file_buffer - total_len;
						if(pthread_create(&th, NULL, deal_upgrade_data, file_buffer) != 0)
						{
							perror("pthread_create");
						}
						continue;
					}
				}
				usleep(30000);		
			}
			
		}
	}
	if( pthread_join(th,&tret) != 0 )
	{
		perror("pthread_join");
		exit(1);	
	}
	return 0;
}
