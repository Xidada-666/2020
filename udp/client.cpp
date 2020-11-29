#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 9999

int main()
{
    int sockFD = socket(AF_INET,SOCK_DGRAM,0);

    struct sockaddr_in ser;
    ser.sin_port   = htons(PORT);
    ser.sin_family = AF_INET    ;
    ser.sin_addr.s_addr = inet_addr("103.100.211.220") ;

    int len = sizeof(struct sockaddr_in);
    char * p = "zhangsan";
    char buf[100];
    while (1)
    {
        memset(buf,0,100);
        ssize_t size = sendto(sockFD,p,8,0,(struct sockaddr*)&ser,len);

        printf("send size %d\n",size);

        size = recvfrom(sockFD,buf,100,0,(struct sockaddr*)&ser,(socklen_t*)&len);

        buf[size] = '\0';
        printf("client: %s\n",buf);

    }
    close(sockFD);
    return 0;
}