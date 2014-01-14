#include <stdio.h>
#include <arpa/inet.h>

int main()
{
	unsigned int len = 0x1234;
	unsigned int len1 = 0;
        unsigned int addr = 0;
	
        len1 = htonl(addr);
        printf("########### htonl(0) = %x.\n", htonl(0));
        
	len1 = htonl(len);
	printf("########### len1 = %x.\n", len1);
	
	len1 = htonl(len1);
	printf("########### len1 = %x.\n", len1);
	
	len1 = ntohl(len);
	printf("########### len1 = %x.\n", len1);
	
	len1 = ntohl(len1);
	printf("########### len1 = %x.\n", len1);
	
	return 0;
}
