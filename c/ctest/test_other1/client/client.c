/**	@file fileclient.c
 *	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief ipc升级客户端 
 *
 *	@author		zhoualei
 *	@date		2012/09/10
 *
 *	@note 实现文件传输，心跳，断网重新传输，文件解析。 
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
 *	@brief	断网重新连接 
 *	@param sockfd 标识一个未连接套接口的描述字.
 *	@param addr 欲进行连接的端口名.
 *	@param alen 名字长度.
 *	@return	0:表示已经连接上. -1:表示连接失败. 
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
 *	@brief	打印客户端进度条的百分比信息 
 *	@param file_recv 表示已经接受的数量 
 *	@param file_total 总数量 
 *	@param spOut 指向信息存放的地址.
 *	@return	N\A 
 */
void drawper(int file_recv, int file_total, char* spOut)
{
	static int iCrlmm = 0;
	int iPer = file_recv * 100 / file_total;
	/*参数检查*/ 
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

/**@brief 利用简单的异或进行数据变换，用于升级文件头的打包和解包
* @param[in] char *p_src, 数据的源地址
* @param[in] int len，数据的长度
* @param[out] char *p_dst, 数据的目的地址
* @return 0 - 成功，-1 - 失败
*/
int convert_data(char *p_src, char *p_dst, int len)
{

   // 固定的幻数，用于异或变换
   UINT8 magic[] = {0xba, 0xcd, 0xbc, 0xfe, 0xd6, 0xca, 0xdd, 0xd3,
   					0xba, 0xb9, 0xa3, 0xab, 0xbf, 0xcb, 0xb5, 0xbe};
   INT32 i = 0;
   INT32 j = 0;
   INT32 magic_len = 0;
   INT32 start_magic = 0; 
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

/**	@fn	UINT32 calculate_checksum_header(char* temp_header_tran, int length)
 *	@brief	校验升级包的报文头 
 *	@param temp_header_tran 升级包头信息 
 *	@param length 升级包头长度 
 *	@return	校验和 
 */
UINT32 calculate_checksum_header(char* temp_header_tran, int length)
{
	int i;
	/* 参数检查*/
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
 *	@brief	校验升级文件 
 *	@param	data_upgrade 升级文件信息 
 *	@param 	filesize 升级文件长度 
 *	@return	校验和 
 */
UINT32 calculate_checksum_upgrade_file(char* data_upgrade, int filesize)
{
	int i;
	/* 参数检查*/
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
 *	@brief	数据处理线程 
 *	@param	arg	接收到的数据包 
 *	@return	N\A
 */
void *deal_upgrade_data(void *arg)
{
	int file_fd;
	FIRMWARE_HEADER *firmware_head = NULL;
	UPGRADE_FILE_HEADER *update_file_head =NULL;
	char *temp_header_tran = NULL;
	/* 参数检查*/
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
	/*对整个升级包的报文头进行解码*/ 
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
	/*通过包头信息转换文件头*/
	if( -1 == convert_data(upgrade_file_buffer, temp_header_tran, FIRMWARE_HEADER_LEN + FILE_HEADER_LEN * firmware_head->file_nums))
	{
		printf("convert error\n");
		pthread_exit(NULL);
	}
	/*校验整个升级包的报文头*/
	if(firmware_head->header_checksum != calculate_checksum_header(temp_header_tran, firmware_head->header_length))
	{
		perror("checksum header");
	//	pthread_exit(NULL);
	}
	
	memcpy(update_file_head, temp_header_tran + FIRMWARE_HEADER_LEN, FILE_HEADER_LEN * firmware_head->file_nums);
	/*升级文件*/
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
		/*获取升级文件名*/ 
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
		/*根据升级文件头信息copy各自的升级文件数据*/ 
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
		/*如果文件已经存在，则不重写*/ 
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
				/*文件已存在，不重写*/
				printf("file %s already exist\n",filename);
				free(rd_buf);
				rd_buf = NULL;
				free(file_rd_buf);
				file_rd_buf = NULL;
				close(wr_file);
				wr_file = 0;
				continue;		/*跳到下个文件处理*/
			}		
			free(file_rd_buf);
			file_rd_buf = NULL;
		}	
		close(wr_file);
		wr_file = 0;
			
		/*文件不同直接写,以清除方式打开*/ 
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
	int size_temp = 0;	/*用于打印进度条*/ 
	
	int sockfd;
	int conn_ret;
	struct sockaddr_in servaddr;
	struct hostent *host;
	char cmd_buf[10];	/*用来存放已经接收了多少数据*/
	char recvbuf[MAX_LEN];
	char file_sz[10];	/*用于存放服务器发来的文件总大小*/
	char cmd_file[20]; /*用来存放*/ 
	int len = MAX_LEN;
	int total_len = 0;
	char *file_buffer = NULL; 
	void *tret;	/*pthread_join需要的参数*/ 
	int filesize;
	FILESIZE fsz;
	CMD_FILE filecmd;
	char beatbuf[5];
	pthread_t th;
	struct timeval start,stop;
	memset(szStr, '=', 100);
	szStr[100] = '\0';
	memset(beatbuf, '\0', sizeof(beatbuf));
	/*检查参数*/ 
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
	/*客户端发送升级请求，并发送已收取的文件大小*/ 
	if(write(sockfd, cmd_buf, sizeof(REQUEST_FROM_CLIENT)) == -1)
	{
		perror("write");
		exit(1);
	}
	/*客户端读取升级文件大小*/ 
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
	printf("\033[?25l");/*去除光标，纯属为了好看*/ 
	while(1)
	{
		gettimeofday(&stop, 0);
		if(read(sockfd, cmd_file, sizeof(CMD_FILE))!= -1)
		{
			memcpy(&filecmd, cmd_file, sizeof(CMD_FILE));
			/*心跳10秒未收到则认为网络异常，重新发起连接*/ 
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
				
				if(!flag)/*flag == 0 表示升级结束发信息给服务器*/ 
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
					/*读取的数据统一存在file_buffer里面*/			
					memcpy(file_buffer, recvbuf, len);
					/*打印进度条信息*/ 
					total_len += len;
					size_temp = 100 * total_len / filesize;
					memset( szOut, 0, sizeof(szOut) );
					szStr[size_temp] = cTmp;
					
					drawper(size_temp, iStrLen, szOut);
					
					memcpy(szStr+iStrLen, szOut, strlen(szOut) );
					fseek(stdout, 0, SEEK_SET);
					fprintf(stdout, szStr);
					fflush(stdout);
					/*记录已经收到的文件大小*/ 
					request_filesize.recev_filesize = total_len;
					file_buffer = file_buffer + len;
					if(total_len == filesize)
					{
						/*已收到的文件大小和需要收取的文件大小一致则创建线程处理数据*/ 
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
