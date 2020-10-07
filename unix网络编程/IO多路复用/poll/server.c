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
#include <sys/poll.h>

// int poll(struct pollfd *fds, nfds_t nfds, int timeout);

// struct pollfd {
//     int fd;/* 文件描述符 */
//     short events;   /* 监控的事件 */
//     short revents;  /* 监控事件中满足条件返回的事件 */
// };
// POLLIN普通或带外优先数据可读,即POLLRDNORM | POLLRDBAND
// POLLRDNORM-数据可读
// POLLRDBAND-优先级带数据可读
// POLLPRI 高优先级可读数据

// POLLOUT普通或带外数据可写
// POLLWRNORM-数据可写
// POLLWRBAND-优先级带数据可写

// POLLERR 发生错误
// POLLHUP 发生挂起
// POLLNVAL 描述字不是一个打开的文件

// nfds 监控数组中有多少文件描述符需要被监控
// timeout 毫秒级等待
//     -1：阻塞等，#define INFTIM  -1 Linux中没有定义此宏
//     0：立即返回，不阻塞进程
//     >0：等待指定毫秒数，如当前系统时间精度不够毫秒，向上取值


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
	
	int retfd = 0;
	struct pollfd client[CLIENT_COUNT];//初始化客户端连接数
	for(int i = 0; i < CLIENT_COUNT; ++i){
		client[i].fd = -1;
	}

	client[0].fd = serverfd;
	client[0].events = POLLIN;
	
	int maxindex = 0;

	while(1){
		if((retfd = poll(client, maxindex + 1, -1)) < 0){
			perror("poll fail\n");
			exit(1);
		}

		if(client[0].revents & POLLIN){//监听描述符有新连接到达
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

			for(int i = 1; i < CLIENT_COUNT; ++i){
				if(client[i].fd == -1){
					client[i].fd = confd;//保存accept返回的描述符到client[]中
					client[i].events = POLLIN;
					maxindex = i;
					printf("client[%d].fd = %d\n", i, client[i].fd);
					break;
				}
			}

			if(--retfd == 0)//
				continue;
		}

		//检测client是否有数据到达
		for(int i = 1; i < CLIENT_COUNT; ++i){
			//printf("client[%d] : %d", i, client[i]);
			if(client[i].fd == -1)
				continue;
			if(client[i].revents & POLLIN){
				//处理数据请求
				char buf[1024];
				int len = read(client[i].fd, buf,
						sizeof(buf));
				if(len == 0){//若客户端关闭了连接
					close(client[i].fd);
					client[i].fd = -1;
				}
				for(int j = 0; j < len; ++j)
					buf[j] = toupper(buf[j]);
				write(client[i].fd, buf, len);

				if(--retfd == 0){
					break;
				}
			}
		}

	}
	close(serverfd);
	return 0;
}



