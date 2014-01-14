
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define NET_BUF_LEN 1024
#define IPADDRESS_STR "CurrentMyIPAddress:"

int main(void)
{
	char tmp_url[NET_BUF_LEN] = {0};
    char ip[NET_BUF_LEN] = {0};
	char *p1 = NULL;
	//char *p2 = NULL;

	strcat(tmp_url, "CurrentMyIPAddress:115.94.72.52.");

	p1 = strstr(tmp_url, IPADDRESS_STR);
	if (p1 != NULL)
	{
		p1 += strlen(IPADDRESS_STR);
		//p2 = strstr(p1, "\0") - 2;
		//if (p2 != NULL)
		//{
			//strncpy(ip, p1, (p2 - p1));
			strncpy(ip, p1, strlen(p1) - 1);
			printf("detected IP: %s\n", ip);
			return 0;
		//}
	}

	return -1;
}

