/*
sererv.c
  * 简单的时间获取服务器
  */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>
 
#define SERVER_PORT 3333   //服务器端口号
#define LEN_OF_LISTEN_QUEUE 10 //最大监听队列数
#define WELCOME_MESSAGE "welcome to commect the server.\n" //欢迎信息
 
int main(int argc, char *argv[])
{
    int servfd, clifd;
    struct sockaddr_in servaddr, cliaddr;
 
    //创建socket
    if( (servfd = socket(AF_INET,SOCK_STREAM,0)) < 0 ) 
    {
        printf("Create socket error!\n");
        exit(1);
    }
 
    //设置服务器地址结构
    bzero(&servaddr,sizeof(servaddr));  
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
 
    //绑定服务器地址
    if( bind(servfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0 )
    {
        printf("Bind to port %d failure!\n", 
                        SERVER_PORT);
        exit(1);
    }
 
    //开始监听
    if( listen(servfd, LEN_OF_LISTEN_QUEUE) < 0 )
    {
        printf("Call listen failure!\n");
        exit(1);
    }
 
    printf("Wait for connect...\n");
 
    while(1)
    {
        char buf[BUFSIZ];
        socklen_t clilen = sizeof(cliaddr);
 
        //接受连接
        if( (clifd = accept(servfd, (struct sockaddr*)&cliaddr, &clilen)) < 0 ) 
        {
            printf("Call accept failure!\n");
            exit(1);
        }
 
        //打印客户端ip及端口号
        printf("connect form %s:%d\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
 
        //整合要发送的信息
        strcpy(buf,WELCOME_MESSAGE);
        strcat(buf,"\nCurrent time is:");
        long currtime = time(NULL);
        strcat(buf,ctime(&currtime));
 
        //发送信息
        send(clifd, buf, BUFSIZ,0);
        close(clifd);
    }
 
    close(servfd);
    return 0;
}
