      
/** @file async_client.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    ��ʾ�����첽ͨѶ�����ǿͻ��˳���
  * 
  * @author   liuboyf1
  * @date     2012-10-15
  * @version  V1.0.0
  * 
  * @note ///Description here 
  * @note History:    
  * @note     <author>   <time>    <version >   <desc>
  * @note  
  * @warning  
  */

#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

#define MAXBUF 1024

int main(int argc, char **argv)
{
    int sockfd, len;
    struct sockaddr_in dest;
    char buffer[MAXBUF + 1];
    fd_set rfds;
    struct timeval tv;
    int retval, maxfd = -1;

    if (argc != 3)
    {
        printf
            ("������ʽ������ȷ�÷����£�\n\t\t%s IP��ַ �˿�\n\t����:\t%s 127.0.0.1 80\n�˳���������ĳ�� IP ��ַ�ķ�����ĳ���˿ڽ������ MAXBUF ���ֽڵ���Ϣ",
             argv[0], argv[0]);
        exit(0);
    }
    /* ����һ�� socket ���� tcp ͨ�� */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket");
        exit(errno);
    }

    /* ��ʼ���������ˣ��Է����ĵ�ַ�Ͷ˿���Ϣ */
    bzero(&dest, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(atoi(argv[2]));
    if (inet_aton(argv[1], (struct in_addr *) &dest.sin_addr.s_addr) == 0)
    {
        perror(argv[1]);
        exit(errno);
    }

    /* ���ӷ����� */
    if (connect(sockfd, (struct sockaddr *) &dest, sizeof(dest)) != 0)
    {
        perror("Connect ");
        exit(errno);
    }

    printf
        ("\n׼�����������Կ�ʼ�����ˡ���ֱ��������Ϣ�س����ɷ���Ϣ���Է�\n");
    while (1)
    {
        /* �Ѽ������ */
        FD_ZERO(&rfds);
        /* �ѱ�׼������0���뵽������ */
        FD_SET(0, &rfds);
        maxfd = 0;
        /* �ѵ�ǰ���Ӿ��sockfd���뵽������ */
        FD_SET(sockfd, &rfds);
        if (sockfd > maxfd)
        {
            maxfd = sockfd;
        }
        /* �������ȴ�ʱ�� */
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        /* ��ʼ�ȴ� */
        retval = select(maxfd + 1, &rfds, NULL, NULL, &tv);
        if (retval == -1)
        {
            printf("���˳���select���� %s", strerror(errno));
            break;
        }
        else if (retval == 0)
        {
            /* printf
               ("û���κ���Ϣ�������û�Ҳû�а����������ȴ�����\n"); */
            continue;
        }
        else
        {
            if (FD_ISSET(sockfd, &rfds))
            {
                /* ���ӵ�socket������Ϣ��������նԷ�����������Ϣ����ʾ */
                bzero(buffer, MAXBUF + 1);
                /* ���նԷ�����������Ϣ�������� MAXBUF ���ֽ� */
                len = recv(sockfd, buffer, MAXBUF, 0);
                if (len > 0)
                {
                    printf
                        ("������Ϣ�ɹ�:'%s'����%d���ֽڵ�����\n",
                         buffer, len);
                }
                else
                {
                    if (len < 0)
                    {
                        printf
                            ("��Ϣ����ʧ�ܣ����������%d��������Ϣ��'%s'\n",
                             errno, strerror(errno));
                    }
                    else
                    {
                        printf("�Է��˳��ˣ�������ֹ��\n");
                    }
                    break;
                }
            }
            if (FD_ISSET(0, &rfds))
            {
                /* �û������ˣ����ȡ�û���������ݷ��ͳ�ȥ */
                bzero(buffer, MAXBUF + 1);
                fgets(buffer, MAXBUF, stdin);
                if (!strncasecmp(buffer, "quit", 4))
                {
                    printf("�Լ�������ֹ���죡\n");
                    break;
                }
                /* ����Ϣ�������� */
                len = send(sockfd, buffer, strlen(buffer) - 1, 0);
                if (len < 0)
                {
                    printf
                        ("��Ϣ'%s'����ʧ�ܣ����������%d��������Ϣ��'%s'\n",
                         buffer, errno, strerror(errno));
                    break;
                } else
                {
                    printf
                        ("��Ϣ��%s\t���ͳɹ�����������%d���ֽڣ�\n",
                         buffer, len);
                }
            }
        }
    }
    /* �ر����� */
    close(sockfd);
    return 0;
}

