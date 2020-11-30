#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define PORT 8889

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

    int keepalive = 1;
    setsockopt(sockFD,SOL_SOCKET,SO_KEEPALIVE,(void*)(&keepalive),(socklen_t)sizeof(keepalive));

    int keepalive_time = 30;
    setsockopt(sockFD, IPPROTO_TCP, TCP_KEEPIDLE,(void*)(&keepalive_time),(socklen_t)sizeof(keepalive_time));
    int keepalive_intvl = 3;
    setsockopt(sockFD, IPPROTO_TCP, TCP_KEEPINTVL,(void*)(&keepalive_intvl),(socklen_t)sizeof(keepalive_intvl));
    int keepalive_probes= 3;
    setsockopt(sockFD, IPPROTO_TCP, TCP_KEEPCNT,(void*)(&keepalive_probes),(socklen_t)sizeof(keepalive_probes));
    //设置SO_KEEPALIVE选项来开启KEEPALIVE，然后通过TCP_KEEPIDLE、TCP_KEEPINTVL和TCP_KEEPCNT设置keepalive的保活时间、间隔、次数等参数。

    if(bind(sockFD,(struct sockaddr*)&addr_serv,sizeof(struct sockaddr_in)) < 0)
    {
        perror("bind\n");
        exit(1);
    }
    printf("bind ok\n");

    char buf[100];
    len = sizeof(struct sockaddr_in);
    struct sockaddr_in client;
    int i  = 0;
    while (1)
    {
        memset(buf,0,100);
        ssize_t size = recvfrom(sockFD,buf,sizeof(buf),0,(struct sockaddr*)&client,(socklen_t*)&len) ;
        if(size<0)
        {
            perror("err\n");
        }
        buf[size] = '\0';
        printf("server: %s\n",buf);
        size = sendto(sockFD,&i,sizeof(int),0,(struct sockaddr*)&client,len);
        i++;
        printf("send size :%d\n",size);
    }

    close(sockFD);
    return 0;
}