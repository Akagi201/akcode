/**	@file rw.c
 *	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief 数据读写封装 
 *
 *	@author		zhoualei
 *	@date		2012/08/30
 *
 *	@note 大文件读写数据包会丢失封装一层读写避免丢包 
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
 *	@brief	封装的读 
 *	@param	fd	要读的文件句柄 
 *	@param	buf 读取的数据存放的位置指针.
 *	@param	len	表示要读取的字节数 
 *	@return	-1表示读取失败。0表示读取成功。 
 */
int readall(int fd, void *buf, int *len)
{
	int nleft;	/*剩下需要读取的字节数*/ 
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
 *	@brief	封装的写 
 *	@param	fd	要写入的文件句柄 
 *	@param	buf 要写的数据存放的位置指针.
 *	@param	len	表示要写入的字节数 
 *	@return	-1表示写入失败。0表示写入成功。 
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
 *	@brief	发送文件和心跳 
 *	@param	sock	存放客户端信息的结构体 
 *	@param	file_name 需要传输的文件的名字 
 *	@return	-1表示写入失败。0表示写入成功。 
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
	UINT8 buffer[MAX_LEN];	/*存放一次发送的数据*/ 
	char file_sz[10];		/*存放要传输的文件总大小用来告知客户端*/ 
	char cmd_file[20];		/*存放数据头结构体*/ 
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
	file_size = file_state.st_size - sock->recev_filesize;/*告知服务器还需要发送多少文件数据*/ 
	sock->send_ready_filesize = sock->recev_filesize;/*已经发送了多少数据用来和显示进度条线程交互*/ 
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
		if((stop.tv_sec - start.tv_sec) >= 5)/*每隔5秒发送一次心跳*/ 
		{
			gettimeofday(&start, 0);
			filecmd.type = 0;
			filecmd.data_size = 4;
			memcpy(cmd_file, &filecmd, sizeof(CMD_FILE));
			error_flag = write(sock->fd, cmd_file, sizeof(CMD_FILE));/*先发数据包头告知客户端是心跳*/ 
			if(error_flag != sizeof(CMD_FILE))
			{
				perror("write");
				return 0;
			}
			if ((error_flag = write(sock->fd, beatbuf, 4)) != 4)/*再发心跳数据*/ 
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
			pthread_rwlock_rdlock(&sock_mutex);/*读写锁的读锁用来修改数据*/ 
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
