// sendto.c
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

void Sendto()
{
    sockaddr_in receiver_addr;
    int sock_fd;
    char line[15] =- "Hello World!";
    sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    receiver_addr.sin_family = AF_INET;
    receiver_addr.sin_port = htons(5000);
    sendto(sock_fd, line, 13, 0, (struct sockaddr *)&receiver_addr, sizeof(receiver_addr));
    close(sock_fd);
}
