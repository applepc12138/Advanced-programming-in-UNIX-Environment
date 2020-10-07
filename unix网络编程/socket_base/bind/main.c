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

// bind()的作用是将参数sockfd和addr绑定在一起，使sockfd这个用于网络通讯的文件描述符监听addr所描述的地址和端口号
// int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
// sockfd：socket文件描述符
// addr: 构造出IP地址加端口号
// addrlen: sizeof(addr)长度
// 返回值：成功返回0，失败返回-1, 设置errno



int main(int argc, char *argv[])
{
	if(socket(AF_INET, SOCK_STREAM, 0) == -1)	
		perror("create socket fail\n");
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;//地址族
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//ipv4地址
	servaddr.sin_port = htons(2048);//端口
	
	return 0;
}
