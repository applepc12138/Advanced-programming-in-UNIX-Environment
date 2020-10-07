/*************************************************************************
	> File Name: client.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年09月24日 星期四 15时46分35秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int clientfd = socket(AF_INET, SOCK_STREAM, 0);
	//初始化服务器地址
	struct sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(2048);
	//连接服务器
	if(connect(clientfd, (struct sockaddr *)&serveraddr,
				sizeof(serveraddr)) == -1)
		perror("connect fail\n");
	
	//数据处理
	if(argc != 2){
		printf("please input two argumaent\n");
		exit(1);
	}
	int len = write(clientfd, argv[1], strlen(argv[1]));
	char buf[4096];
	read(clientfd, buf, len);
	printf("result is %s\n", buf);
	
	//关闭socket
	close(clientfd);
	return 0;
}

