#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <math.h>
#include <sys/select.h>
#include <sys/time.h>
 
#define SERVER_PORT 3333  //服务器端口号
 
void str_cli(FILE *fp, int sockfd)
{
    int maxfdp1, stdineof;
    fd_set rset;
    char buf[BUFSIZ];
    int n;
 
    stdineof = 0;
    FD_ZERO(&rset);
 
    while(1)
    {
        if( stdineof == 0 )
            FD_SET(fileno(fp),&rset);
        FD_SET(sockfd, &rset);
 
        maxfdp1 = ((fileno(fp) > sockfd) ? fileno(fp) : sockfd) + 1;
 
        select(maxfdp1, &rset, NULL, NULL, NULL);
 
        if( FD_ISSET(sockfd, &rset) )
        {
            if( (n = read(sockfd, buf, BUFSIZ)) == 0 )
                if( stdineof == 1 )
                    return;
                else
                    perror("server terminated prematurely");
            write(fileno(stdout), buf, n);
        }
 
        if( FD_ISSET(fileno(fp), &rset))
        {
            if( (n = read(fileno(fp), buf, BUFSIZ)) == 0 )
            {
                stdineof = 1;
                shutdown(sockfd, SHUT_WR);
                FD_CLR(fileno(fp), &rset);
                continue;
            }
            write(sockfd, buf, n);
        }
    }
}
 
int main(int argc, char *argv[])
{
    int sockfd[5];
    struct sockaddr_in servaddr;
    struct hostent  *hp;
    char buf[BUFSIZ];
 
    if( argc != 2 )
    {
        printf("Please input %s <hostname>\n", argv[0]);
        exit(1);
    }
     
    int i;
    for(i = 0; i < 5; ++i)
    {
 
        //创建socket
        if( (sockfd[i] = socket(AF_INET, SOCK_STREAM,0)) < 0 )
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
        if( connect(sockfd[i],(struct sockaddr*)&servaddr, sizeof(servaddr)) < 0 )
        {
            printf("Connect server failure!\n");
            exit(1);
        }
    }
    str_cli(stdin, sockfd[0]);
 
    exit(0);
}
