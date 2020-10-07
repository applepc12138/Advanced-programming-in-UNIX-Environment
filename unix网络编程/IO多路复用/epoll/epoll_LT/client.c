/*************************************************************************
	> File Name: client.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年10月02日 星期五 09时44分44秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
	//创建socket
	int confd = socket(AF_INET, SOCK_STREAM, 0);
	
	//连接服务器
	struct sockaddr_in serversock;
	serversock.sin_family = AF_INET;
	serversock.sin_addr.s_addr = htonl(INADDR_ANY);
	serversock.sin_port = htons(8888);
	if(connect(confd, (struct sockaddr *)&serversock, 
				sizeof(serversock)) < 0){
		perror("connect fail");
		exit(1);
	}
	char buf[128];
	while(fgets(buf, sizeof(buf), stdin) != NULL){
		write(confd, buf, strlen(buf));
		int len = read(confd, buf, sizeof(buf));		
		if(len > 0)
			write(STDOUT_FILENO, buf, len);
		
	}
	return 0;
}

