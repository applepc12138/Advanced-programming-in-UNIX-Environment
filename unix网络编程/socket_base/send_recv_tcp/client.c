/*************************************************************************
	> File Name: client.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年09月26日 星期六 15时32分07秒
 ************************************************************************/

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

int main(int argc, char *argv[])
{
	//创建套接字
	int clientfd;
	if((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("create socket fail\n");
		exit(1);
	}
	//连接服务器
	struct sockaddr_in serversock;
	bzero(&serversock, sizeof(serversock));
	serversock.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &serversock.sin_addr.s_addr);
	//serversock.sin_addr.s_addr = htonl(INADDR_ANY);
	serversock.sin_port = htons(2048);
	if(connect(clientfd, (struct sockaddr *)&serversock,
				sizeof(serversock)) == -1){
		perror("connect fail\n");
		exit(1);
	}
	//数据传输
	if(argc != 2){
		printf("please input two argument\n");
		exit(1);
	}
	int len = send(clientfd, argv[1], strlen(argv[1]), 0);
	char buf[128];
	recv(clientfd, buf, len, 0);
	printf("buf :%s\n", buf);
	
	return 0;
}


