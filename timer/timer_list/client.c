/*************************************************************************
	> File Name: client.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年10月10日 星期六 10时22分49秒
 ************************************************************************/

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int confd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in serversock;
	bzero(&serversock, sizeof(serversock));
	serversock.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &serversock.sin_addr);
	serversock.sin_port = htons(65535);
	if(connect(confd, (struct sockaddr *)&serversock, 
			sizeof(serversock)) < 0){
		perror("connect server fail\n");
		exit(1);
	}
	
	char buf[128];
	while(fgets(buf, sizeof(buf), stdin) != NULL){
		send(confd, buf, strlen(buf), 0);
		int len = recv(confd, buf, sizeof(buf), 0);
		if(len > 0)
			write(STDOUT_FILENO, buf, len);
	}

	return 0;
}

