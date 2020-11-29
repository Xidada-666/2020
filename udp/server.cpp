#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#define PORT 9998
int main()
{
    int sockFD = socket(AF_INET,SOCK_DGRAM,0);
    if(sockFD<0)
    {
        perror("socket\n");
        exit(1);
    }

    struct sockaddr_in addr_serv;
    int len;
    memset(&addr_serv,0,sizeof(struct sockaddr_in));
    addr_serv.sin_port = htons(PORT);
    addr_serv.sin_family = AF_INET;
    addr_serv.sin_addr.s_addr = htonl(INADDR_ANY);

    int flag = 1;
    setsockopt(sockFD,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(flag));
    setsockopt(sockFD,SOL_SOCKET,SO_REUSEPORT,&flag,sizeof(flag));

    if(bind(sockFD,(struct sockaddr*)&addr_serv,sizeof(struct sockaddr_in)) < 0)
    {
        perror("bind\n");
        exit(1);
    }

    char buf[100];
    len = sizeof(struct sockaddr_in);
    struct sockaddr_in client;
    while (1)
    {
        memset(buf,0,100);
        ssize_t size = recvfrom(sockFD,buf,sizeof(buf),0,(struct sockaddr*)&client,(socklen_t*)&len) ;
        buf[size] = '\0';

        printf("server: %s\n",buf);

        char *p ="000123";
        sendto(sockFD,p,6,0,(struct sockaddr*)&client,len);
    }
    close(sockFD);
    return 0;
}