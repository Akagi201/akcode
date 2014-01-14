/**	@file rw.c
 *	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief ���ݶ�д��װ 
 *
 *	@author		zhoualei
 *	@date		2012/08/30
 *
 *	@note ���ļ���д���ݰ��ᶪʧ��װһ���д���ⶪ�� 
 *	@note 
 */
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<sys/time.h>
#include"rw.h"
#include"upgrade_file.h"
/**	@fn	int readall(int fd, void *buf, int *len)
 *	@brief	��װ�Ķ� 
 *	@param	fd	Ҫ�����ļ���� 
 *	@param	buf ��ȡ�����ݴ�ŵ�λ��ָ��.
 *	@param	len	��ʾҪ��ȡ���ֽ��� 
 *	@return	-1��ʾ��ȡʧ�ܡ�0��ʾ��ȡ�ɹ��� 
 */
int readall(int fd, void *buf, int *len)
{
	int nleft;	/*ʣ����Ҫ��ȡ���ֽ���*/ 
	int nread;
	int total;
	char *ptr;
	ptr = buf;
	nleft = *len;
	total = 0;
	
	while(nleft > 0)
	{
		if((nread = read(fd, ptr, *len)) == -1)
		{
			perror("readall");
			break;
		}
		if(nread == 0)
		{
			break;
		}
		nleft -= nread;
		ptr += nread;
		total +=nread;
		*len =nleft;
	}
	*len = total;
	return (nread == -1) ? -1 : 0;
}

/**	@fn	int writeall(int fd, void *buf, int *len)
 *	@brief	��װ��д 
 *	@param	fd	Ҫд����ļ���� 
 *	@param	buf Ҫд�����ݴ�ŵ�λ��ָ��.
 *	@param	len	��ʾҪд����ֽ��� 
 *	@return	-1��ʾд��ʧ�ܡ�0��ʾд��ɹ��� 
 */
int writeall(int fd, void *buf, int *len)
{
	int nleft;
	int nwrite;
	int total;
	const char *ptr;
	ptr = buf;
	nleft = *len;
	total = 0;
	
	while(nleft > 0)
	{
		if((nwrite = write(fd, ptr, *len)) == -1)
		{
			perror("writeall");
			break;
		}
		nleft -= nwrite;
		ptr += nwrite;
		total +=nwrite;
		*len =nleft;
	}
	*len = total;
	return (nwrite == -1) ? -1 : 0;
}
/**	@fn	int send_file(SOCKET *sock, char *file_name)
 *	@brief	�����ļ������� 
 *	@param	sock	��ſͻ�����Ϣ�Ľṹ�� 
 *	@param	file_name ��Ҫ������ļ������� 
 *	@return	-1��ʾд��ʧ�ܡ�0��ʾд��ɹ��� 
 */
int send_file(SOCKET *sock, char *file_name)
{
	int file_fd;
	int file_size;
	int read_left;
	int len;
	int error_flag;
	int readlen;
	char beatbuf[4]="heat";
	struct stat file_state;
	UINT8 buffer[MAX_LEN];	/*���һ�η��͵�����*/ 
	char file_sz[10];		/*���Ҫ������ļ��ܴ�С������֪�ͻ���*/ 
	char cmd_file[20];		/*�������ͷ�ṹ��*/ 
	struct timeval start,stop;
	
	if((file_fd = open(file_name, O_RDONLY)) == -1)
	{
		perror("open");
		return -1;
	}
	if(fstat(file_fd, &file_state) == -1)
	{
		perror("fstat");
		return -1;
	}
	sock->filesize_total = file_state.st_size;
	file_size = file_state.st_size - sock->recev_filesize;/*��֪����������Ҫ���Ͷ����ļ�����*/ 
	sock->send_ready_filesize = sock->recev_filesize;/*�Ѿ������˶���������������ʾ�������߳̽���*/ 
	lseek(file_fd, sock->recev_filesize, SEEK_SET);
	FILESIZE fsz;
	fsz.filesize = file_size;
	memcpy(file_sz, &fsz, sizeof(FILESIZE));
	error_flag = write(sock->fd, file_sz, sizeof(FILESIZE));
	if(error_flag != sizeof(FILESIZE))
	{
		perror("write");
		return -1;
	}
	read_left =file_size;
	len =MAX_LEN;
	
	gettimeofday(&start, 0);
	CMD_FILE filecmd;
	filecmd.type = 0;
	filecmd.data_size = 0;
	while(sock->pthread_flag)
	{
		gettimeofday(&stop, 0);
		if((stop.tv_sec - start.tv_sec) >= 5)/*ÿ��5�뷢��һ������*/ 
		{
			gettimeofday(&start, 0);
			filecmd.type = 0;
			filecmd.data_size = 4;
			memcpy(cmd_file, &filecmd, sizeof(CMD_FILE));
			error_flag = write(sock->fd, cmd_file, sizeof(CMD_FILE));/*�ȷ����ݰ�ͷ��֪�ͻ���������*/ 
			if(error_flag != sizeof(CMD_FILE))
			{
				perror("write");
				return 0;
			}
			if ((error_flag = write(sock->fd, beatbuf, 4)) != 4)/*�ٷ���������*/ 
     		{
     			perror("write");
     			return 0;
			}
			
		}
		if(read_left > 0)
		{
			filecmd.type = 1;
			if(read_left > MAX_LEN)
			{
				filecmd.data_size = MAX_LEN;
				
			}
			else
			{
				filecmd.data_size = read_left;
			}
			memcpy(cmd_file, &filecmd, sizeof(CMD_FILE));
			error_flag = write(sock->fd, cmd_file, sizeof(CMD_FILE));
			if(error_flag != sizeof(CMD_FILE))
			{
				perror("write");
				return -1;
			}
			readlen = filecmd.data_size;
			error_flag = readall(file_fd, buffer, &readlen);
			if(error_flag < 0)
			{
				return -1;
			}
			read_left -= readlen;
			len = readlen;
			error_flag = writeall(sock->fd, buffer, &len);
			if(error_flag == -1)
			{
				return -1;
			}
			pthread_rwlock_rdlock(&sock_mutex);/*��д���Ķ��������޸�����*/ 
			sock->send_ready_filesize = sock->send_ready_filesize + filecmd.data_size;
			pthread_rwlock_unlock(&sock_mutex);
			//sleep(1);
			if(read_left == 0)
			{
				close(file_fd);
			}	
		}
	}
	return 0;

}
