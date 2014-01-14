// send.c

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

void Recv()
{
    struct sockaddr_in serv_addr;
    int sock_fd;
    char line[15] = "Hello world!";
    int size = 13;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    serv_addr.sin_port = htons(5000);
    sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    send(sock_fd, line, size, 0);
    close(sock_fd);
}
