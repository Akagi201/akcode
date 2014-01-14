/**@file arp_ip_check.c
 * @note HangZhou Hikvision Digital Co, Ltd. All Right Reserved.
 * @brief  ͨ���㲥ARP �������IP��ͻ��
 * 
 * @author   ����
 * @date     2012-9-17
 * @version  V1.0
 * 
 * @note ///Description here 
 * @note History:        
 * @note     <author>   <time>    <version >   <desc>
 * @note  
 * @warning  
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <errno.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
                
#include <netinet/in.h> 
#include <netinet/if_ether.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <arpa/inet.h>  
                
#include "arp_ip_check.h"
        
#define MAC_BCAST_ADDR      (unsigned char *) "/xff/xff/xff/xff/xff/xff"
#define ETH_INTERFACE       "eth0"
            
struct server_config_t server_config;

/**@brief	���Ŀ��IP �Ƿ��ͻ  
 * @param[in]  addr: Ŀ��IP ��ַ
 * @param[out] 
 * @return	  
 */
int check_ip(u_int32_t addr)// addr ��Ҫ����ip
{
    struct in_addr temp;

    if (arpping(addr, server_config.server, server_config.arp, ETH_INTERFACE) == 0)
    {
        temp.s_addr = addr;
        printf("%s belongs to someone, reserving it for %ld seconds\n",
            inet_ntoa(temp), server_config.conflict_time);
        return 1;
    }
    else
        return 0;
}
// ����:Ŀ��ip������ip������mac����������
int arpping(u_int32_t yiaddr, u_int32_t ip, unsigned char *mac, char *interface)
{
	int timeout = 2;
    int optval = 1;
    int arp_socket;                      /* socket */
    int rv = 1;                 /* return value */
    struct sockaddr addr;       /* for interface name */
    struct arpMsg arp;
    fd_set fdset;
    struct timeval tm;
    time_t prevTime;
	int ret;

  	/*socket����һ��arp��*/
    if ((arp_socket = socket (PF_PACKET, SOCK_PACKET, htons(ETH_P_ARP))) == -1)
	{
        printf("Could not open raw socket\n");
        return -1;
    }
    
    /*�����׽ӿ�����Ϊ�㲥�������arp���ǹ㲥�����������*/
    if (setsockopt(arp_socket, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) == -1)
	{
        printf("Could not setsocketopt on raw socket\n");
        close(arp_socket);
        return -1;
    }

	/* ��ʼ��ARP ��*/
    memset(&arp, 0, sizeof(arp));
    memcpy(arp.ethhdr.h_dest, MAC_BCAST_ADDR, 6);   /* Ŀ����̫����ַ */
    memcpy(arp.ethhdr.h_source, mac, 6);        /* Դ��̫����ַ */
    arp.ethhdr.h_proto = htons(ETH_P_ARP);
    arp.htype = htons(ARPHRD_ETHER);
    arp.ptype = htons(ETH_P_IP); 
    arp.hlen = 6;
    arp.plen = 4;
    arp.operation = htons(ARPOP_REQUEST);
    *((u_int *) arp.sInaddr) = ip;
    memcpy(arp.sHaddr, mac, 6);
    *((u_int *) arp.tInaddr) = yiaddr;

    memset(&addr, 0, sizeof(addr));
    strcpy(addr.sa_data, interface);
	
	/*����arp����*/
    if (sendto(arp_socket, &arp, sizeof(arp), 0, &addr, sizeof(addr)) < 0)
    {
		printf("send arp packet err!\n");
		rv = 0;
    }

	/* ����select�������ж�·�ȴ�*/
	tm.tv_usec = 0;
    time(&prevTime);
    while (timeout > 0) 
	{
		FD_ZERO(&fdset);
        FD_SET(arp_socket, &fdset);
        tm.tv_sec = timeout;
        if (select(arp_socket + 1, &fdset, (fd_set *) NULL, (fd_set *) NULL, &tm) < 0)
		{
			printf("Error on ARPING request: %s\n", strerror(errno));
            if (errno != EINTR)
			{
				printf("select fail\n");
				rv = 0;
			}
        }
		else if (FD_ISSET(arp_socket, &fdset))
		{
            if (recv(arp_socket, &arp, sizeof(arp), 0) < 0 )
            {
                rv = 0;
			}
			/* ������� htons(ARPOP_REPLY) bcmp(arp.tHaddr, mac, 6) == 0 *((u_int *) arp.sInaddr) == yiaddr ���߶�Ϊ�棬
			��ARPӦ����Ч,˵�������ַ���ѽ����ڵ�*/
            if (arp.operation == htons(ARPOP_REPLY) 
				&& bcmp(arp.tHaddr, mac, 6) == 0 
				&& *((u_int *) arp.sInaddr) == yiaddr)
                {
                	printf("Valid arp reply receved for this address\n");
                	rv = 0;
                	break;
            	}
        }
		
        timeout -= time(NULL) - prevTime;
        time(&prevTime);
    }
    close(arp_socket);
    return rv;
}

/*�����ֱ��ʾ �����豸���� �ӿڼ������� ����IP��ַ ����arp��ַ*/

int read_interface(char *interface, int *ifindex, u_int32_t *addr, unsigned char *arp)
{
    int fd;
    /*ifreq�ṹ������/usr/include/net/if.h����������ip��ַ������ӿڣ�����MTU�Ƚӿ���Ϣ�ġ�
    ���а�����һ���ӿڵ����ֺ;������ݡ������Ǹ������壬�п�����IP��ַ���㲥��ַ��
    �������룬MAC�ţ�MTU���������ݣ���
    ifreq������ifconf�ṹ�С���ifconf�ṹͨ���������������нӿڵ���Ϣ�ġ�
    */

    struct ifreq ifr;
    struct sockaddr_in *our_ip;

    memset(&ifr, 0, sizeof(struct ifreq));
	
    /*����һ��socket������SOCK_RAW��Ϊ�˻�ȡ������������IP�����ݣ�
     	 *	IPPROTO_RAW�ṩӦ�ó�������ָ��IPͷ���Ĺ��ܡ�
    	 **/
    if((fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) >= 0)
	{
        ifr.ifr_addr.sa_family = AF_INET;
                /*���������͸�ֵ��ifr_name*/
        strcpy(ifr.ifr_name, interface);

        if (addr)
		{
               /*SIOCGIFADDR���ڼ����ӿڵ�ַ*/
            if (ioctl(fd, SIOCGIFADDR, &ifr) == 0)
			{
                  /*��ȡ����IP��ַ��addr��һ��ָ��õ�ַ��ָ��*/
                our_ip = (struct sockaddr_in *) &ifr.ifr_addr;
                *addr = our_ip->sin_addr.s_addr;
                printf("%s (our ip) = %s\n", ifr.ifr_name, inet_ntoa(our_ip->sin_addr));
            }
			else
			{
                printf("SIOCGIFADDR failed, is the interface up and configured?: %s\n",
                        strerror(errno));
                return -1;
            }
        }

         /*SIOCGIFINDEX���ڼ����ӿ�����*/
        if (ioctl(fd, SIOCGIFINDEX, &ifr) == 0)
		{
            printf("adapter index %d\n", ifr.ifr_ifindex);
             /*ָ��ifindex ��ȡ����*/
            *ifindex = ifr.ifr_ifindex;
        }
		else
        {
            printf("SIOCGIFINDEX failed!: %s\n", strerror(errno));
            return -1;
        }
		
        /*SIOCGIFHWADDR���ڼ���Ӳ����ַ*/
        if (ioctl(fd, SIOCGIFHWADDR, &ifr) == 0)
		{
             /*����ȡ��Ӳ����ַ���Ƶ��ṹserver_config������arp[6]������*/
            memcpy(arp, ifr.ifr_hwaddr.sa_data, 6);
            printf("adapter hardware address %02x:%02x:%02x:%02x:%02x:%02x\n",
                arp[0], arp[1], arp[2], arp[3], arp[4], arp[5]);
        }
		else
		{
            printf("SIOCGIFHWADDR failed!: %s\n", strerror(errno));
            return -1;
        }
    }
    else
	{
        printf("socket failed!: %s\n", strerror(errno));
        return -1;
    }
    close(fd);
    return 0;
}

int main(int argc, char *argv[])
{       
	int ret;
    if(argc < 2)
    {   
        printf("Usage: checkip ipaddr\n");
        exit(0);
    }
    
      /*����̫���ӿں�������ȡһЩ������Ϣ*/
    if (read_interface(ETH_INTERFACE, &server_config.ifindex,
               &server_config.server, server_config.arp) < 0)
    {
        exit(0);
    }
    
   /*IP��⺯��*/
    if((ret = check_ip(inet_addr(argv[1]))) == 0)
    {
    	printf("ret == %d\n", ret);
        printf("IP:%s can use\n", argv[1]); 
    }
    else
    {
    	printf("ret == %d\n", ret);
        printf("IP:%s conflict\n", argv[1]);
    }
    
    return 0;
}
