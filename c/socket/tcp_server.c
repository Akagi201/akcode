
/** Server code in C
 * Setting up a simple TCP server involves the following steps:
 * 1. Creating a TCP socket, with a call to socket().
 * 2. Binding the socket to the listen port, with a call to bind(). Before calling bind(), a programmer must declare a sockaddr_in structure, clear it (with memset()), and the sin_family (AF_INET), and fill its sin_port (the listening port, in network byte order) fields. Converting a short int to network byte order can be done by calling the function htons() (host to network short).
 * 3. Preparing the socket to listen for connections (making it a listening socket), with a call to listen().
 * 4. Accepting incoming connections, via a call to accept(). This blocks until an incoming connection is received, and then returns a socket descriptor for the accepted connection. The initial descriptor remains a listening descriptor, and accept() can be called again at any time with this socket, until it is closed.
 * 5. Communicating with the remote host, which can be done through send() and recv() or write() and read().
 * 6. Eventually closing each socket that was opened, once it is no longer needed, using close().
 * */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

int main(void)
{
    struct sockaddr_in stSockAddr;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (-1 == SocketFD)
    {
        perror("can not create socket");
        exit(EXIT_FAILURE);
    }

    memset(&stSockAddr, 0, sizeof(stSockAddr));

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(1100);
    stSockAddr.sin_addr.s_addr = INADDR_ANY;

    if (-1 == bind(SocketFD,(struct sockaddr *)&stSockAddr, sizeof(stSockAddr)))
    {
        perror("error bind failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    if(-1 == listen(SocketFD, 10))
    {
        perror("error listen failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    for(;;)
    {
        int ConnectFD = accept(SocketFD, NULL, NULL);

        if(0 > ConnectFD)
        {
            perror("error accept failed");
            close(SocketFD);
            exit(EXIT_FAILURE);
        }

        /* perform read write operations ...
           read(ConnectFD,buff,size)*/
        sleep(10);

        if (-1 == shutdown(ConnectFD, SHUT_RDWR))
        {
            perror("can not shutdown socket");
            close(ConnectFD);
            exit(EXIT_FAILURE);
        }
        close(ConnectFD);
    }

    return EXIT_SUCCESS;
}


