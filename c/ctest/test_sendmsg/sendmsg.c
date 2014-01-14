// sendmsg.c
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

void sendmsg()
{
    struct sockaddr_in receiver_addr;
    int sock_fd;
    char line[15] = "Hello World!";
    struct msghdr msg;
    struct lovec iov;
    sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    receiver_addr.sin_family = AF_INET;
    receiver_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    receiver_addr.sin_port = htons(5000);
    msg.msg_name = &receiver_addr;
    msg.msg_namelen = sizeof(receiver_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_iov->iov_len = 13;
    msg.msg_control = 0;
    msg.msg_controllen = 0;
    msg.msg_flags = 0;
    sendmsg(sock_fd, &msg, 0);
    close(sock_fd);
}
