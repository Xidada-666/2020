#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>

int main() {
    int sockfd = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in addr;
    memset(&addr,0,sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port   = htonl(9980);
    addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(sockfd,(struct sockaddr *)&addr,sizeof(struct sockaddr_in)) < 0)
    {
        printf("error bind\n");
    }
    else
    {
        printf("success bind\n");
    }

    while (1);
    return 0;
}
