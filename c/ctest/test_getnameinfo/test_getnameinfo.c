
#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    struct sockaddr_in6 sa;   // IPv6
    struct sockaddr_in sa_v4; // IPv4
    char host[1024];
    char service[20];

    // pretend sa is full of good information about the host and port...

    getnameinfo(&sa, sizeof sa, host, sizeof host, service, sizeof service, 0);

    printf("   host: %s\n", host);    // e.g. "www.example.com"
    printf("service: %s\n", service); // e.g. "http"

    return 0;
}
