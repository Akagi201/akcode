
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

void echo(int connfd);
void *thread(void *vargp);

int main(int argc, char *argv[])
{
    int listenfd;
    int *connfdp;
    int port;
    int clientlen = sizeof(struct sockaddr_in);
    struct sockaddr_in clientaddr;

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    port = atoi(argv[1]);

    listenfd = open_listenfd(port);
    while(1)
    {
        connfdp = malloc(sizeof(int));
        *connfdp = accept(listenfd, (SA *) &clientaddr, &clientlen);
        pthread_create(&tid, NULL, thread, connfdp);
    }
}

void *thread(void *vargp)
{
    int connfd = *((int *)vargp);
    pthread_detach(pthread_self());
    free(vargp);
    echo(connfd);
    close(connfd);
    return NULL;
}

