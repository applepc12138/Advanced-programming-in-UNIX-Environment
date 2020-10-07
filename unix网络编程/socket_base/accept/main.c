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
#include <unistd.h>
#include <errno.h>

// int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
// 服务器调用accept()时还没有客户端的连接请求，就阻塞等待直到有客户端连接上来
// sockdf:socket文件描述符addr:传出参数，返回链接客户端地址信息，含IP地址和端口号
// addrlen:传入传出参数（值-结果）,传入sizeof(addr)大小，函数返回时返回真正接收到地址结构体的大小
// 返回值：成功返回一个新的socket文件描述符，该描述符连接到调用connect的客户端，失败返回-1，设置errno

int main(int argc, char *argv[])
{
	int servsockfd;
	if((servsockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)	
		perror("create socket fail\n");
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;//地址族
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//ipv4地址
	servaddr.sin_port = htons(8000);//端口

	if(bind(servsockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
		perror("bind fail\n");
	
	if(listen(servsockfd, 128) == -1)
		perror("listen fail\n");
	
	while(1){
		printf("in while\n");
		struct sockaddr_in clientaddr;
		socklen_t client_addrlen = sizeof(clientaddr);
		int confd = accept(servsockfd, (struct sockaddr*)&clientaddr, &client_addrlen);
		//输出客户端ip地址，端口号
		//int hostaddr = ntohl(clientaddr.sin_addr.s_addr);
		char addr[32];
		inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, addr, sizeof(addr));	
		printf("client address is : %s\tport is :%d\n",
				addr, ntohs(clientaddr.sin_port));
		
		close(confd);
	}

	return 0;
}
