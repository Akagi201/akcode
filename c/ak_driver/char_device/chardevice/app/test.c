      
/**@file test.c
 * @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @brief  测试程序
 * 
 * @author   liuboyf1
 * @date     2012-8-30
 * @version  V1.0.0
 * 
 * @note ///Description here 
 * @note History:        
 * @note     <author>   <time>    <version >   <desc>
 * @note  
 * @warning  
 */


#include <unistd.h> 
#include <stdio.h>
#include <string.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <pthread.h>
#include <sys/time.h> 
#include <stdlib.h> 


// Ioctl 命令编码
#define CHRDEV_IOC_MAGIC  't'                                  // 幻数
#define CHRDEV_IOC_BUFCLEAR    _IO(CHRDEV_IOC_MAGIC, 0)        // 清除设备缓存为0
#define CHRDEV_IOC_SIZE	   _IOR(CHRDEV_IOC_MAGIC, 1, int)      // 查询当前的数据总量，直接返回数据

#ifdef DEBUG_PRINT
#define DBG_PRINTFILE_LINE(void) do{printf("\nFN: %s, FUNC: %s, L: %d ", __FILE__, __func__,__LINE__);}while(0)
#define DBG_PRINT(fmt,args...)  do {DBG_PRINTFILE_LINE(void); printf(fmt,##args);} while(0)
#define ERR_PRINT(str) do { DBG_PRINTFILE_LINE(void); perror(str);}while(0)
#else
#define DBG_PRINT(fmt, args...) do {}while (0)
#define ERR_PRINT(str)          do {}while (0)
#endif
#define CYCLE_CTRL 500
#define INPUT_CHARS "adsfsdfsadfsdfsa"
#define RBUF_SIZE 0x00200000             // 读缓存区长度
#define PER_READ_SIZE 0x00200000         // 每次读取的字符长度
#define PER_WRITE_SIZE 0x00200000        // 每次写入的字符长度
#define DELAY 10                          // 写线程每次写入延时的毫秒数
#define DEVICE_NAME "/dev/chrdev"


/** @struct 
 *  @brief 向写线程传递参数的结构体
 */
typedef struct
{
	char *write_chars;           // 写者每次写入的字符内容
	unsigned int write_size;           // 写者每次写入的长度
	char * dev_name;             // 字符设备名字
	int total_wlen;              // 循环写入的次数
	unsigned int delay;                // 延迟的毫秒数
	
}Writer, *PWriter;

/** @struct 
 *  @brief 向读线程传递参数的结构体
 */

typedef struct
{
	char * read_buf;              // 读缓存区
	char * dev_name;              // 字符设备名字
	int read_size;                // 每次读取的长度
	int total_rlen;               // 总共需要读取的字符长度

}Reader, *PReader;

/** @fn
  * @brief	    ioctl测试
  * @param[in]  dev_name : 虚拟字符设备的名字
  * @param[out]
  * @return 0 : 成功 -1 : 失败
  */
int ioctl_test(char * dev_name)
{
	int i_ret = 0;
	int fd = 0;
	int cur_size = 0;

	if(NULL == dev_name)
	{
		DBG_PRINT("[Fail]dev_name is NULL!\n");
		return -1;
	}
	// 打开准备读取的设备
	fd = open(dev_name, O_RDWR);
	if(-1 == fd)
	{
		ERR_PRINT("[Fail]open() error!\n");
		return -1 ;
	}

	// 测试CHRDEV_IOC_BUFCLEAR（清零设备缓存）
	i_ret = ioctl(fd, CHRDEV_IOC_BUFCLEAR); 
	if(0 == i_ret)
	{
		DBG_PRINT("Testing ioctl()!\n");
	}
	else
	{
		close(fd);
		return -1;
	}

	// 测试CHRDEV_IOC_SIZE 获得设备缓存中当前字符数
	cur_size = ioctl(fd, CHRDEV_IOC_SIZE); 
	if(-1 != cur_size)
	{
		DBG_PRINT("Testing ioctl(): get cur_size:%d\n", cur_size);
	}
	else
	{
		close(fd);
		return -1;
	}

	close(fd);
	return 0;
}


/** @fn
  * @brief	    计算指定时间间隔，指定传输大小的传输字符的速度
  * @param[in]  read_bits : 读取的字节数 begin : 开始的时间值 end : 结束的时间值
  * @param[out]
  * @return     0 : 成功 -1 : 失败
  */
int compute_read_rate(unsigned int read_bits, struct timeval begin, struct timeval end)
{
	unsigned int delay_time = 0;                  // 临时时间间隔值

    if (end.tv_sec >= begin.tv_sec)
    {
        delay_time = (end.tv_sec - begin.tv_sec) *1000;
        delay_time += (end.tv_usec - begin.tv_usec)/1000;
        delay_time = (delay_time < 0) ? (0 - delay_time): (delay_time);
    }
    else
    {
        delay_time = (begin.tv_sec - end.tv_sec) * 1000 ;
        delay_time += (begin.tv_usec - end.tv_usec)/1000 ;
    }

	delay_time -= DELAY * CYCLE_CTRL;
	DBG_PRINT("\n\n read_bits :%d  waste time :%d\n",read_bits , delay_time );
	if(delay_time > 0)
	{
		printf("read_bits :%d , waste time : %d read rate is :%.2f KB/ms", read_bits, delay_time
			   ,(float)(read_bits>>10)/(delay_time));
		return 0;
	}
	return -1;

}

/** @fn
  * @brief	    读线程函数，实现对设备的阻塞读，并计算读取的速度值
  * @param[in]  lparam : 消息机制参数
  * @param[out]
  * @return
  */
void thread_reader(void *lparam)
{
	PReader stru_read;
	int fd = 0;
	int all_count = 0;
	fd_set rset;
	int i_ret = 0;
	unsigned int tmp_count = 0;
	struct timeval begin_time;
	struct timeval end_time;

	if(NULL == lparam)
	{
		DBG_PRINT("[Fail]lparam is NULL!\n");
		return;
	}
	stru_read = (PReader)lparam;

	DBG_PRINT("thread_reader():get dev_name: %s\n", stru_read->dev_name);
	FD_ZERO(&rset);
	all_count = stru_read->total_rlen; 

	// 打开准备读取的设备
	fd = open(stru_read->dev_name, O_RDONLY);
	if(-1 == fd)
	{
		ERR_PRINT("[Fail]open() error! \n");
		return ;
	}

	gettimeofday(&begin_time, NULL);
	printf("thread_reader():begin time : tv_sec:%u  tv_usec:%u\n", (unsigned int )begin_time.tv_sec
			  , (unsigned int )begin_time.tv_usec);
	while(all_count > 0)//
	{
		//测试poll 
		FD_SET(fd, &rset);

		// 查看当前设备是否可读
		i_ret = select((fd + 1), &rset, NULL, NULL, NULL);
		if(-1 == i_ret)
		{
			ERR_PRINT("[Fail]select() failed\n");
			close(fd);
			return;
		}

		i_ret = read(fd, stru_read->read_buf, stru_read->read_size);
		if(-1 == i_ret)
		{
			ERR_PRINT("[Fail]read() error !\n");
			close(fd);
			return ;
		}
		stru_read->read_buf[i_ret] = '\0';
		tmp_count += i_ret;
		all_count -= i_ret;
		DBG_PRINT("thread_reader(): have read bytes :%d  all_count :%u tmp_count: %u\n\n", i_ret
				  , all_count, tmp_count);
	}
	gettimeofday(&end_time, NULL);
	printf("thread_reader(): end time : tv_sec:%u  tv_usec:%u\n", (unsigned int )end_time.tv_sec
			  , (unsigned int )end_time.tv_usec);

	i_ret = compute_read_rate(stru_read->total_rlen, begin_time, end_time);
	if(-1 == i_ret)
	{
		DBG_PRINT("[Fail]compute_read_rate() failed\n");
	}

	close(fd);
	return ;
}

/** @fn
  * @brief	    写线程函数，实现对设备的阻塞读，并计算写入的速度值
  * @param[in]  lparam : 消息机制参数
  * @param[out]
  * @return
  */
void thread_writer(void *lparam)
{
	PWriter stru_write;
	int fd = 0;
	int i_ret = 0;
	unsigned int tmp_count = 0;
	int all_count_w = 0;

	if(NULL == lparam)
	{
		DBG_PRINT("[Fail]lparam is NULL!\n");
		return;
	}
	stru_write = (PWriter)lparam;

	all_count_w = stru_write->total_wlen;
	DBG_PRINT("thread_writer(): get dev_name: %s\n", stru_write->dev_name);
	// 打开准备写入的设备
	fd = open(stru_write->dev_name, O_WRONLY);
	if(-1 == fd)
	{
		ERR_PRINT("[Fail]open() error! \n");
		return ;
	}

	printf("thread_writer() : delay :%u ms\n", stru_write->delay); 
	while(all_count_w > 0)
	{
		if(all_count_w > stru_write->write_size)
		{
			i_ret = write(fd, stru_write->write_chars, stru_write->write_size);
		}
		else
		{
			i_ret = write(fd, stru_write->write_chars, all_count_w);
		}
		if(-1 == i_ret)
		{
			ERR_PRINT("[Fail]write() error !\n");
			close(fd);
			return ;
		}
		
		tmp_count += i_ret;
		all_count_w -= i_ret;
		DBG_PRINT("thread_writer() :have write bytes :%d all_count :%d tmp_count: %u\n\n", i_ret
				  , all_count_w, tmp_count);
		usleep(1000 * stru_write->delay);
	}
	close(fd);

	return ;
}

/** @fn
  * @brief	    通过循环拷贝生成指定长度的字符
  * @param[in]  gen_len : 想要生成的字符的长度
  * @param[out] out_str : 生成的字符指针
  * @return     0：成功； -1：失败
  */
int generate_chars(char * out_str, int gen_len)
{
	int total_count = gen_len;
	int cpy_len = 0;
	int input_chars_len = strlen(INPUT_CHARS);

	while(total_count > 0)
	{
		if(total_count > input_chars_len)
		{
			memcpy(out_str + cpy_len, INPUT_CHARS, input_chars_len);
			total_count -= input_chars_len;
			cpy_len += input_chars_len;
		}
		else
		{
			memcpy(out_str + cpy_len, INPUT_CHARS, total_count);
			cpy_len += total_count;
			total_count -= total_count;
		}
	}

	if(gen_len != cpy_len)
	{
		return -1;
	}
	return 0;
}

int main()
{
	int i_ret = 0;
	pthread_t read_thread;
	pthread_t write_thread;
	Reader stru_read;
	Writer stru_write;
	char buf_in[PER_WRITE_SIZE + 1 ] = {0};
	char *dev_name = DEVICE_NAME;
	char read_buffer[RBUF_SIZE + 1] = {0};
	void * thread_result_r = NULL;
	void * thread_result_w = NULL;

	i_ret = generate_chars(buf_in, PER_WRITE_SIZE);
	if(-1 == i_ret)
	{
		DBG_PRINT("generate_chars() failed\n");
		return -1;
	}
	// 准备读写线程的输入参数
	stru_read.dev_name = dev_name;
	stru_read.read_size = PER_READ_SIZE;
	stru_read.total_rlen = PER_WRITE_SIZE * CYCLE_CTRL;
	stru_write.total_wlen = PER_WRITE_SIZE * CYCLE_CTRL;
	stru_write.dev_name = dev_name;
	stru_write.write_chars = buf_in;
	stru_write.write_size = PER_WRITE_SIZE;
	stru_write.delay = DELAY;                           // 毫秒
	stru_read.read_buf = read_buffer;

	i_ret = pthread_create(&read_thread, NULL, (void *)&thread_reader, (void *)&stru_read);
	if(i_ret != 0)
	{
		DBG_PRINT("[Fail]pthread_create() for thread_reader() failed\n");
		//return -1;
	}

	i_ret = pthread_create(&write_thread, NULL, (void *)&thread_writer, (void *)&stru_write);
	if(i_ret != 0)
	{
		DBG_PRINT("[Fail]pthread_create() for thread_writer() failed\n");
		//return -1;
	}
	i_ret = pthread_join(read_thread, &thread_result_r);
	if(i_ret != 0)
	{
		DBG_PRINT("[Fail]pthread_join() for thread_reader() failed\n");
		//return -1;
	}

	i_ret = pthread_join(write_thread, &thread_result_w);
	if(i_ret != 0)
	{
		DBG_PRINT("[Fail]pthread_join() for thread_writer() failed\n");
		//return -1;
	}

	DBG_PRINT("Test Finished!\n");
	
	return 0;	
}

