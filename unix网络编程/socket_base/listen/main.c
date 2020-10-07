/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年09月23日 星期三 10时04分32秒
 ************************************************************************/

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <strings.h>
#include <arpa/inet.h>
#include <errno.h>

// int listen(int sockfd, int backlog);
// sockfd:socket文件描述符
// backlog:排队建立3次握手队列和刚刚建立3次握手队列的链接数和

int main(int argc, char *argv[])
{
	int servsockfd;
	if((servsockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)	
		perror("create socket fail\n");
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;//地址族
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//ipv4地址
	servaddr.sin_port = htons(2048);//端口
	
	if(listen(servsockfd, 5) == -1)
		perror("listen fail\n");
	


	return 0;
}
