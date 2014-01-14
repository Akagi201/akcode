/*	@file ctest.c
 *	@note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *	@brief �ַ��������Ĳ��Գ���:�ṩ�ַ��������ٶȲ��ԣ�����buff����ѵ���Թ��ܡ�
 *	@author qinhuatao
 *	@date 2011/10/26
 *	@note
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

#define BUFSIZE 0x80000
#define TARGET "/dev/ctest"			/* �豸·�� */
#define DATALEN  0x20000000			/* �ٶȲ������ݳ��� 512MB */
#define CTEST_MEM_CLEAR 0x1 		/* ioctl���� */
#define DELAY 20                  /* ��ʱms */

int circle = 0;

#define dbg_line do{printf("debug-------<%s><%d>\n",__FUNCTION__,__LINE__);}while(0)

/**	@fn	void *produce(void * total_len)
 *	@brief	ѭ��д����
 *	@param �Ѿ�д��������ֽ���
 *	@return	NULL
 */
void *produce(void *id)
{
    int dev_fd;
    int num = 0;
    int need_len;
    int complete_len;
    int ret;
    char *buf = NULL;

    dbg_line;

    buf = (char *)malloc(BUFSIZE);

    if (NULL == buf)
    {
        perror("buf is NULL\n");
        return NULL;
    }

    /* ���д���� */
    memset(buf, 51, BUFSIZE);
    dev_fd = open(TARGET, O_WRONLY);

    if (dev_fd <= 0)
    {
        perror("can't open device file\n");
        return NULL;
    }

    while (num < DATALEN)
    {
        need_len = BUFSIZE;
        complete_len = 0;

        while (need_len != (ret = write(dev_fd, buf + complete_len, need_len)))
        {
            need_len -= ret;
            complete_len += ret;
        }

        num += complete_len;
        circle++;
        usleep(DELAY * 1000);
    }

    close(dev_fd);
    return NULL;
}

/**	@fn	void *produce(void * total_len)
 *	@brief	ѭ��������
 *	@return	NULL
 */
void *cunsume(void *id)
{
    int dev_fd;
    //char buf[BUFSIZE];
    int num = 0;
    struct timeval start, stop;
    dbg_line;
    dev_fd = open(TARGET, O_RDWR);

    if (dev_fd <= 0)
    {
        perror("can't open device file\n");
        return NULL;
    }

    dbg_line;
    gettimeofday(&start, NULL);

    while (num < DATALEN)
    {
        num += read(dev_fd, NULL , BUFSIZE);
        //printf("num = %d\n",num);
    }

    gettimeofday(&stop, NULL);
    close(dev_fd);

    /* �������� */
    read_rate(&start, &stop, num);

    return NULL;
}

/**	@fn	int clear_dev_buf(void)
 *	@brief	ioctl���Բ��֣�����buff����
 *	@return	0 ����; -1 ����
 */
int clear_dev_buf(void)
{
    int dev_fd, ret;
    dbg_line;
    dev_fd = open(TARGET, O_RDWR);

    if (dev_fd <= 0)
    {
        perror("can't open device file\n");
        return -1;
    }

    /* ���豸�������� */
    ret = ioctl(dev_fd, CTEST_MEM_CLEAR, NULL);

    if (ret != CTEST_MEM_CLEAR)
    {
        printf("clear error\n");
    }
    else
    {
        printf("buff has been clear\n");
    }

    close(dev_fd);
    return 0;
}

/**	@fn	void test_poll(void)
 *	@brief	��ѵ���Ʋ��Գ���
 *	@return
 */
void test_poll(void)
{
    int count, dev_fd, stringlen;
    char buf[BUFSIZE];
    fd_set wset, rset;

    dbg_line;
    dev_fd = open(TARGET, O_RDWR | O_NONBLOCK);

    if (dev_fd < 0)
    {
        perror("can't open the file\n");
        return ;
    }

    while (1)
    {
        FD_ZERO(&wset);
        FD_ZERO(&rset);
        FD_SET(dev_fd, &rset);
        FD_SET(dev_fd, &wset);

        /* ��ѯ */
        select(dev_fd + 1, &rset, &wset, NULL, NULL);

        if (FD_ISSET(dev_fd, &rset))
        {
            count = read(dev_fd, buf, BUFSIZE - 1);
            buf[count] = '\0';
            printf("%s", buf);

        }

        if (FD_ISSET(dev_fd, &wset))
        {
            sprintf(buf, "produce--!!\n");
            stringlen = strlen(buf);

            if (stringlen != write(dev_fd, buf, stringlen))
            {
                perror("produce write error\n");
                break;
            }
        }
    }

    close(dev_fd);
}

int read_rate(struct timeval *begin, struct timeval *end, int total_len)
{
    int msec;
    struct timeval diff;
    float rate;

    if ( begin->tv_sec > end->tv_sec )
    {
        return -1;
    }

    if ((begin->tv_sec == end->tv_sec) && (begin->tv_usec > end->tv_usec))
    {
        return -1;
    }

    diff.tv_sec = ( end->tv_sec - begin->tv_sec );
    diff.tv_usec = ( end->tv_usec - begin->tv_usec );

    if (diff.tv_usec < 0)
    {
        diff.tv_sec--;
        diff.tv_usec += 1000000;
    }

    msec = (diff.tv_sec * 1000) + (diff.tv_usec / 1000) - circle * DELAY;
    printf("data_len = %d\n", total_len);
    printf("spending = %d ms\n", msec);
    rate = ((float)(total_len >> 10)) / msec;
    printf("=================\nrate = %.2fKB/ms\n", rate);
    return 0;
}


/**	@fn	int main(int argc, char **argv)
 *	@brief	���Գ������
 *	@param argc �����������
 *	@param argv ���������
 *	@return	0���� ��-1����
 */
int main(int argc, char **argv)
{
    pthread_t tid_produce1, tid_cunsume;

    if (argc < 2)
    {
        printf("usge:<cmd> <mode>;mode:1---test read speed ,2---test poll, 3---clear buf\n");
        return -1;
    }

    if (!strcmp(argv[1], "1"))
    {
        dbg_line;
        pthread_create(&tid_produce1, NULL, produce, NULL);
        //sleep(3);
        pthread_create(&tid_cunsume, NULL, cunsume, NULL);
        //pthread_join(tid_produce1, NULL); /*�ȴ������߽���*/
        //pthread_join(tid_produce2, NULL); /*�ȴ������߽���*/
        pthread_join(tid_cunsume, NULL); /*�ȴ������߽���*/


        printf("end\n");
    }

    if (!strcmp(argv[1], "2"))
    {
        dbg_line;
        test_poll();

        printf("test end\n");
    }

    if (!strcmp(argv[1], "3"))
    {
        dbg_line;
        clear_dev_buf();

    }

    return 0;
}



