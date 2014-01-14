
/** Client code in C
 * Programming a TCP client application involves the following steps:
 * 1. Creating a TCP socket, with a call to socket().
 * 2. Connecting to the server with the use of connect(), passing a sockaddr_in structure with the sin_family set to AF_INET, sin_port set to the port the endpoint is listening (in network byte order), and sin_addr set to the IP address of the listening server (also in network byte order.)
 * 3. Communicating with the server by using send() and recv() or write() and read().
 * 4. Terminating the connection and cleaning up with a call to close().
 * */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
    struct sockaddr_in stSockAddr;
    int Res;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (-1 == SocketFD)
    {
        perror("cannot create socket");
        exit(EXIT_FAILURE);
    }

    memset(&stSockAddr, 0, sizeof(stSockAddr));

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(1100);
    Res = inet_pton(AF_INET, "127.0.0.1", &stSockAddr.sin_addr);

    if (0 > Res)
    {
        perror("error: first parameter is not a valid address family");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }
    else if (0 == Res)
    {
        perror("char string (second parameter does not contain valid ipaddress)");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    if (-1 == connect(SocketFD, (struct sockaddr *)&stSockAddr, sizeof(stSockAddr)))
    {
        perror("connect failed");
        close(SocketFD);
        exit(EXIT_FAILURE);

    }

    /* perform read write operations ... */

    shutdown(SocketFD, SHUT_RDWR);
    /*
     if (-1 == shutdown(ConnectFD, SHUT_RDWR))
     {
        perror("Can not shutdown socket.");
        close(SocketFD);
        exit(EXIT_FAILURE);
     }
     */

    close(SocketFD);
    return EXIT_SUCCESS;
}

