/*************************************************************************
	> File Name: server.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年09月25日 星期五 20时10分57秒
 ************************************************************************/

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <strings.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int socketfd;
	//创建socket
	if((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		perror("create socket fail\n");
	//绑定ip地址
	struct sockaddr_in servsock;
	bzero(&servsock, sizeof(servsock));
	servsock.sin_family = AF_INET;
	servsock.sin_addr.s_addr = htonl(INADDR_ANY);
	servsock.sin_port = htons(2048);
	if(bind(socketfd, (struct sockaddr *)&servsock, sizeof(servsock)) < 0)
		perror("bind socket fail\n");
	//设置监听队列
	if(listen(socketfd, 128) < 0){
		perror("set listen queue fail\n");
		exit(1);
	}
	//阻塞等待客户端请求
	struct sockaddr_in clientsock;
	while(1){
		socklen_t clientsock_len = sizeof(clientsock);
		int confd;
		if((confd = accept(socketfd, (struct sockaddr *)&clientsock, &clientsock_len)) < 0)
			perror("accept fail\n");
		//打印客户端ip端口号
		char clientaddr[32];
		printf("client ip: %s\tport%d\n",
				inet_ntop(AF_INET, &clientsock.sin_addr.s_addr, clientaddr, sizeof(clientaddr)), ntohs(clientsock.sin_port));

		//处理数据
		char buf[128];
		int len = recv(confd, buf, sizeof(buf), 0);
		for(int i = 0; i< len; ++i){
			buf[i] = toupper(buf[i]);
		}
		send(confd, buf, len, 0);

		close(confd);
}
	return 0;
}

