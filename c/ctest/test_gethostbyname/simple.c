
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>

int main(void)
{
    struct hostent *h;
    h = gethostbyname("ddns.fujiko.biz");
    //h = gethostbyname("127.0.0.1");
    if (NULL == h)
    {
        herror("gethostbyname.");
        exit(1);
    }
    printf("%s\n", h->h_name);
    printf("%d\n", h->h_addr);
    struct in_addr *in = {h->h_addr};
    //printf("%s\n", inet_ntoa(*in));
    //printf("IP Address: %s\n", inet_ntoa(*((struct in_addr *)h->h_addr)));
    return EXIT_SUCCESS;
}


