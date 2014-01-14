/**@file  server_heart.c
 * @note  HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief �������е���������ĸ����������� 
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

#include "server_heart.h"

/**@brief  �����źŴ�����������ʱ��ͻ��˷���һ������
* @param signo
* @return  ��
*/
static void sig_alrm(int signo)
{    
    int i = 0;
	int err = 0;	 
	char c = 'A';
	
	usleep(200000);	
	err = send(ser_sock, &c, 1, MSG_OOB);			
	if(err < 0)
	{
		//�ͻ��˶�ʧ���˳����ͽ���
		printf("\n\t\t����:�ͻ����ѹرգ��˳����ͽ���\n");
		exit(0);
	}		
	usleep(200000);	
	
	if(heart_count++ > max_times)
	{
		printf("\n\t\t����:�ͻ����Ѷ�ʧ���˳����ͽ���\n");
	}
	
	//��������
	alarm(alarm_time);
	return ;
}


/**@brief  TCP�����źŴ�����������ʱ��ʾ���յ�һ��������Ӧ����������ʧ����������
* @param signo
* @return  ��
*/
static void sig_urg(int signo)
{
	char c = 0;
	int err = 0;
	
	err = recv(ser_sock, &c, 1, MSG_OOB);
	if(err < 0)
	{
		printf("\n��������: recv error\n");
	}
	
	heart_count = 0;
	fflush(stdout);	
}

/**@brief  SIGCHLD�źŴ������������ӽ���
* @param signo
* @return  ��
*/
void sig_chld(int signo)
{
	pid_t pid = 0;
	int stat = 0;
	
	while((pid = waitpid(-1, &stat, WNOHANG)) > 0)
	{
		printf("\n\t\t�����ӽ���<%d> ����ֹ��\n", pid);		
	}
	return ;
}

/**@brief  ���������ʼ��
* @param sock �׽���������
* @param maxtime_arg ���Ӵ���ʱ�䣨��λ���룩
* @param maxtime_arg ���������ʧ��
* @return  ��
*/
void heartbeat_serv(int sock_arg, int alarm_time_arg, int max_times_arg)
{
	ser_sock = sock_arg; 
	alarm_time = alarm_time_arg;
	max_times = max_times_arg;
	heart_count = 0;
	
	//���׽���������Ϊ��ǰ����
	fcntl(ser_sock, F_SETOWN, getpid());
	
	signal(SIGALRM, sig_alrm);	
	signal(SIGURG, sig_urg);
	alarm(alarm_time);
	return;
}


