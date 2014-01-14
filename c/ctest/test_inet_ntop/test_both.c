
#include <string.h> // bzero memset
#include <netinet/in.h> // struct in_addr
#include <sys/socket.h> // AF_INET
#include <arpa/inet.h>

int main(int argc, char **argv)
{
    char str[] = "172.8.5.111";
    char str1[16];
    struct in_addr myin;
    int ret = 1;
    unsigned long lip = 0xac100ac4;

    ret = inet_pton(AF_INET, str, &myin); // 转换出来的myin是网络序的
    printf("%2x\n", myin.s_addr);
    myin.s_addr = ntohl(myin.s_addr);
    printf("%2x\n", myin.s_addr);

    printf("%2x\n", lip);
    lip = htonl(lip);                     // 要先将lip转换成网络序
    printf("%2x\n", lip);
    ret = inet_ntop(AF_INET, &lip, str1, 16);
    printf("%s\n", str1);

    return 0;
}

/*
 * inet_pton的输出是网络序的
 * inet_ntop的输入是网络序的
 */
