      
/** @file async-server.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    ��ʾ�����첽ͨѶ�����Ƿ������˳���
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
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/types.h>

#define MAXBUF 1024

int main(int argc, char **argv)
{
    int sockfd, new_fd;
    socklen_t len;
    struct sockaddr_in my_addr, their_addr;
    unsigned int myport, lisnum;
    char buf[MAXBUF + 1];
    fd_set rfds;
    struct timeval tv;
    int retval, maxfd = -1;

    if (argv[1])
    {
        myport = atoi(argv[1]);
    }
    else
    {
        myport = 7838;
    }

    if (argv[2])
    {
        lisnum = atoi(argv[2]);
    }
    else
    {
        lisnum = 2;
    }

    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = PF_INET;
    my_addr.sin_port = htons(myport);
    if (argv[3])
    {
        my_addr.sin_addr.s_addr = inet_addr(argv[3]);
    }
    else
    {
        my_addr.sin_addr.s_addr = INADDR_ANY;
    }

    if (bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr))
        == -1)
    {
        perror("bind");
        exit(1);
    }

    if (listen(sockfd, lisnum) == -1)
    {
        perror("listen");
        exit(1);
    }

    while (1) {
        printf("\n----�ȴ��µ����ӵ�����ʼ��һ�����졭��\n");
        len = sizeof(struct sockaddr);
        if ((new_fd =
                accept(sockfd, (struct sockaddr *) &their_addr,
                    &len)) == -1)
        {
            perror("accept");
            exit(errno);
        } else
        {
            printf("server: got connection from %s, port %d, socket %d\n",
                inet_ntoa(their_addr.sin_addr),
                ntohs(their_addr.sin_port), new_fd);
        }

        /* ��ʼ����ÿ���������ϵ������շ� */
        printf("\n׼�����������Կ�ʼ�����ˡ���ֱ��������Ϣ�س����ɷ���Ϣ���Է�\n");
        while (1)
        {
            /* �Ѽ������ */
            FD_ZERO(&rfds);
            /* �ѱ�׼������0���뵽������ */
            FD_SET(0, &rfds);
            maxfd = 0;
            /* �ѵ�ǰ���Ӿ��new_fd���뵽������ */
            FD_SET(new_fd, &rfds);
            if (new_fd > maxfd)
            {
                maxfd = new_fd;
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
                if (FD_ISSET(0, &rfds))
                {
                    /* �û������ˣ����ȡ�û���������ݷ��ͳ�ȥ */
                    bzero(buf, MAXBUF + 1);
                    fgets(buf, MAXBUF, stdin);
                    if (!strncasecmp(buf, "quit", 4))
                    {
                        printf("�Լ�������ֹ���죡\n");
                        break;
                    }
                    len = send(new_fd, buf, strlen(buf) - 1, 0);
                    if (len > 0)
                    {
                        printf
                            ("��Ϣ:%s\t���ͳɹ�����������%d���ֽڣ�\n",
                             buf, len);
                    }
                    else
                    {
                        printf
                            ("��Ϣ'%s'����ʧ�ܣ����������%d��������Ϣ��'%s'\n",
                             buf, errno, strerror(errno));
                        break;
                    }
                }
                if (FD_ISSET(new_fd, &rfds))
                {
                    /* ��ǰ���ӵ�socket������Ϣ��������նԷ�����������Ϣ����ʾ */
                    bzero(buf, MAXBUF + 1);
                    /* ���տͻ��˵���Ϣ */
                    len = recv(new_fd, buf, MAXBUF, 0);
                    if (len > 0)
                    {
                        printf
                            ("������Ϣ�ɹ�:'%s'����%d���ֽڵ�����\n",
                             buf, len);
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
                            printf("�Է��˳��ˣ�������ֹ\n");
                        }
                        break;
                    }
                }
            }
        }
        close(new_fd);
        /* ����ÿ���������ϵ������շ����� */
        printf("��Ҫ����������������(no->�˳�)");
        fflush(stdout);
        bzero(buf, MAXBUF + 1);
        fgets(buf, MAXBUF, stdin);
        if (!strncasecmp(buf, "no", 2))
        {
            printf("��ֹ���죡\n");
            break;
        }
    }

    close(sockfd);
    return 0;
}

