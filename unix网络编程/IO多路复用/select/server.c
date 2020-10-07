/*************************************************************************
	> File Name: server.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年09月27日 星期日 17时59分14秒
 ************************************************************************/

#include <stdio.h>
#include <sys/socket.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <stdlib.h>
#include <strings.h>
#include <netinet/in.h>
#include <sys/time.h>

// int select(int nfds, fd_set *readfds, fd_set *writefds,fd_set *exceptfds, struct timeval *timeout);
//     nfds: 监控的文件描述符集里最大文件描述符加1，因为此参数会告诉内核检测前多少个文件描述符的状态
//     readfds：监控有读数据到达文件描述符集合，传入传出参数
//     writefds：监控写数据到达文件描述符集合，传入传出参数
//     exceptfds：监控异常发生达文件描述符集合,如带外数据到达异常，传入传出参数
//     timeout：定时阻塞监控时间，3种情况
//         1.NULL，永远等下去
//         2.设置timeval，等待固定时间
//         3.设置timeval里时间均为0，检查描述字后立即返回，轮询
// struct timeval {
//     long  tv_sec;/* seconds */
//     long  tv_usec;/* microseconds */
// };
// 把文件描述符集合里fd清0
// void FD_CLR(int fd, fd_set *set);
// 测试文件描述符集合里fd是否置1
// int FD_ISSET(int fd, fd_set *set);
// 把文件描述符集合里fd位置1void 
// void FD_SET(int fd, fd_set *set);
// 把文件描述符集合里所有位清0
// FD_ZERO(fd_set *set);

#define CLIENT_COUNT 100

int main(int argc, char *argv[])
{
	//创建socket
	int serverfd;
	if((serverfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("create socket fail\n");
		exit(1);
	}
	//绑定ip端口号
	struct sockaddr_in serversock;
	bzero(&serversock, sizeof(serversock));
	serversock.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &serversock.sin_addr.s_addr);
	serversock.sin_port = htons(6666);
	if(bind(serverfd, (struct sockaddr *)&serversock, sizeof(serversock)) < 0){
		perror("bind server ip fail\n");
		exit(1);
	}
	//设置监听队列
	listen(serverfd, 12);
	
	fd_set reset;//读描述符集 
	FD_ZERO(&reset);
	FD_SET(serverfd, &reset);//构造描述符集
	
	int retfd = 0;
	int maxfd = serverfd + 1;
	int client[CLIENT_COUNT];//初始化客户端连接数
	for(int i = 0; i < CLIENT_COUNT; ++i){
		client[i] = -1;
	}

	int maxi = -1;
	while(1){
		if((retfd = select(maxfd, &reset, NULL, NULL, NULL)) < 0){
			perror("select fail\n");
			exit(1);
		}
		if(FD_ISSET(serverfd, &reset)){//监听描述符有新连接到达
			//打印客户端ip端口
			struct sockaddr_in clientsock;
			socklen_t clientsocklen = sizeof(clientsock);
			int confd;
			if((confd = accept(serverfd, (struct sockaddr *)&clientsock, &clientsocklen)) < 0){
				perror("accept fail\n");
				exit(1);
			}
			char clientaddr[32];
			printf("new connect client ip:%s\t port:%d\n",
					inet_ntop(AF_INET, 
						&clientsock.sin_addr.s_addr, clientaddr, sizeof(clientaddr)), ntohs(clientsock.sin_port));

			for(int i = 0; i < CLIENT_COUNT; ++i){
				if(client[i] == -1){
					client[i] = confd;//保存accept返回的描述符到client[]中
					printf("client[%d] = %d\n", i, client[i]);
					break;
				}
			}

			FD_SET(confd, &reset);//添加新连接到监控描述符集中
			if(confd > maxfd){
				maxfd = confd;
			}
			printf("106 line \n");
			printf("retfd = %d\n", retfd);
			if(--retfd == 0)//
				continue;
		}
		printf("fffffff\n");
		//检测client是否有数据到达
		for(int i = 0; i < CLIENT_COUNT; ++i){
			printf("client[%d] : %d", i, client[i]);
			if(client[i] == -1)
				continue;
			if(FD_ISSET(client[i], &reset)){
				//处理数据请求
				char buf[1024];
				printf("server reading\n");
				int len = read(client[i], buf,
						sizeof(buf));
				printf("read over\n");
				if(len == 0){//若客户端关闭了连接
					close(client[i]);
					FD_CLR(client[i], &reset);
					client[i] = -1;
				}
				for(int j = 0; j < len; ++j)
					buf[j] = toupper(buf[j]);
				write(client[i], buf, len);

				if(--retfd == 0){
					break;
				}
			}
		}

	}
	close(serverfd);
	return 0;
}



