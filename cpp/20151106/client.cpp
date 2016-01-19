#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
# include <arpa/inet.h>
 
#define SERVER_PORT 3333  //服务器端口号
 
int main(int argc, char *argv[])
{
    int servfd;
    struct sockaddr_in servaddr;
    struct hostent  *hp;
    char buf[BUFSIZ];
 
    if( argc != 2 )
    {
        printf("Please input %s <hostname>\n", argv[0]);
        exit(1);
    }
 
    //创建socket
    if( (servfd = socket(AF_INET, SOCK_STREAM,0)) < 0 )
    {
        printf("Create socket error!\n");
        exit(1);
    }
 
    //设置服务器地址结构
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    if( (hp = gethostbyname(argv[1])) != NULL )
    {
        bcopy(hp->h_addr, (struct sockaddr*)&servaddr.sin_addr, hp->h_length);
    }
    else if(inet_aton(argv[1], &servaddr.sin_addr) < 0 )
    {
        printf("Input Server IP error!\n");
        exit(1);
    }
    servaddr.sin_port = htons(SERVER_PORT);
 
    //连接服务器
    if( connect(servfd,(struct sockaddr*)&servaddr, sizeof(servaddr)) < 0 )
    {
        printf("Connect server failure!\n");
        exit(1);
    }
 
    //接收并打印信息
    recv(servfd,buf,BUFSIZ,0);
    printf("\n%s\n",buf);
 
    close(servfd);
    return 0;
}
