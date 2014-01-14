/**@file  client_heart.c
 * @note  HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief �ͻ����е���������ĸ����������� 
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

#include "client_heart.h"

/**@brief  �����źŴ�����������ʱ��ʾ��������ʧ
* @param signo
* @return  ��
*/
static void sig_alrm(int signo)
{	
	isRecv = 0;
	return;		
}

/**@brief  TCP�����źŴ�����������ʱ��ʾ���յ�һ����������Ӧ��������������
* @param signo
* @return  ��
*/
static void sig_urg(int signo)
{		
	char buf[12];	
	char c = 'R';
	int err = 0;
	
	bzero(buf, sizeof(buf));
	if(recv(cli_sock, buf, sizeof(buf), MSG_OOB) < 0)
	{
		printf("\nsig_urg :recv error\n");
		return ;
	}
	
	usleep(200000);
	err = send(cli_sock, &c, 1, MSG_OOB);			
	if(err < 0)
	{
		//�ͻ��˶�ʧ���˳����ͽ���
		printf("\n\t\tsig_urg: send error\n");
		exit(0);
	}
	usleep(200000);		
	
	// 10S			
	alarm(maxtime);	
	return ;
}

/**@brief  ���������ʼ��
* @param sock �׽���������
* @param maxtime_arg ���Ӵ���ʱ�䣨��λ���룩
* @return  ��
*/
void heartbeat_cli(int sock, int maxtime_arg)
{		
	cli_sock = sock;
	maxtime = maxtime_arg;
	
	//���׽���������Ϊ��ǰ����
	fcntl(sock, F_SETOWN, getpid());
	signal(SIGURG, sig_urg);
	signal(SIGALRM, sig_alrm);
	
	alarm(maxtime);
	return;
}

