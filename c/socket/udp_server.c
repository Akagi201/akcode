
/**
 * The User Datagram Protocol (UDP) is a connectionless protocol with no guarantee of delivery.
 * UDP packets may arrive out of order, multiple times, or not at all.
 *  Because of this minimal design, UDP has considerably less overhead than TCP.
 *  Being connectionless means that there is no concept of a stream or permanent connection between two hosts.
 *  Such data are referred to as datagrams (Datagram Sockets).
 *  UDP address space, the space of UDP port numbers (in ISO terminology, the TSAPs), is completely disjoint(不相交) from that of TCP ports.
 * */

/**
 * Code may set up a UDP server on port 7654
 * */

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h> /* for close() for socket */
#include <stdlib.h>

int main(void)
{
    int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    struct sockaddr_in sa;
    char buffer[1024];
    ssize_t recsize;
    socklen_t fromlen;

    memset(&sa, 0, sizeof sa);
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = INADDR_ANY;
    sa.sin_port = htons(7654);
    fromlen = sizeof(sa);

    if (-1 == bind(sock,(struct sockaddr *)&sa, sizeof(sa)))
    {
        perror("error bind failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    for (;;)
    {
        printf ("recv test....\n");
        recsize = recvfrom(sock, (void *)buffer, sizeof(buffer), 0, (struct sockaddr *)&sa, &fromlen);
        if (recsize < 0) {
            fprintf(stderr, "%s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        printf("recsize: %ld\n ", recsize);
        sleep(1);
        printf("datagram: %.*s\n", (int)recsize, buffer);
    }
}

