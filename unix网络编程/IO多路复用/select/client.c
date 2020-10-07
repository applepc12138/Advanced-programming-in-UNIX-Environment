/*************************************************************************
	> File Name: client.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年09月29日 星期二 18时54分52秒
 ************************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

int main(int argc, char *argv[])
{
	int confd;
	//创建socket
	if((confd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("create socket fail\n");
		exit(1);
	}
	//发送连接请求
	struct sockaddr_in serversock;
	bzero(&serversock, sizeof(serversock));
	serversock.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &serversock.sin_addr.s_addr);
	serversock.sin_port = htons(6666);
	if(connect(confd, (struct sockaddr *)&serversock, sizeof(serversock)) < 0){
		perror("connect fail\n");
		exit(1);
	}
	//数据处理请求
	if(send(confd, argv[1], strlen(argv[1]), 0) < 0){
		perror("client send() fail\n");
		exit(1);
	}
	printf("send over\n");
	char buf[48];
	int len = recv(confd, buf, sizeof(buf), 0);
	printf("recv over\n");

	if(len > 0)
		write(STDOUT_FILENO, buf, len);

	printf("over\n");

	return 0;
}

