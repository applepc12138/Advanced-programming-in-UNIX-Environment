/*************************************************************************
	> File Name: cgi_server.h
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年11月02日 星期一 20时10分31秒
 ************************************************************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdio>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include <sys/epoll.h>
#include <cstdlib>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "../processpool/processpool.h"

//用来处理客户CGI请求的类，可以作为processpool类的模板参数
class cgi_conn
{
	public:
		cgi_conn() = default;
		~cgi_conn() = default;

	public:
		//初始化客户连接，清空缓冲区,sockfd用来与客户端通信
		void init(int epfd, int sockfd, const sockaddr_in& client_addr)
		{
			epollfd = epfd;
			this->sockfd = sockfd;
			this->address = client_addr;
			memset(this->buf, '\0', BUFFER_SIZE);
			read_index = 0;
		}

		void process()
		{
			int index = 0;
			int ret = -1;
			while(true){
				index = this->read_index;
				ret = recv(this->sockfd, this->buf + index, BUFFER_SIZE-1-index, 0);
				if(ret < 0){
					if(errno != EAGAIN)//读操作发生错误，关闭连接
						removefd(this->epollfd, this->sockfd);
					break;//暂时无数据可读，退出循环
				}
				else if(ret == 0){//客户关闭连接，则服务器也关闭连接
					removefd(this->epollfd, this->sockfd);
					break;
				}
				else{
					this->read_index += ret;
					printf("user content is: %s\n", this->buf);

					//遇到字符"\r\n",则开始处理客户请求
					for( ; index < this->read_index; ++index)
						if((index >= 1 && this->buf[index-1] == '\r') && (this->buf[index] == '\n'))
							break;
					
					//未遇到字符"\r\n",需要读取更多的客户数据
					if(index == this->read_index)
						continue;
					this->buf[index-1] = '\0';

					char *file_name = this->buf;
					if(access(file_name, F_OK) == -1){
						removefd(this->epollfd, this->sockfd);
						break;
					}
					//创建子进程来执行CGI程序
					if((ret = fork()) == -1){
						removefd(this->epollfd, this->sockfd);
						break;
					}
					if(ret > 0){
						removefd(this->epollfd, this->sockfd);
						break;
					}	
					else if(ret == 0){
						close(STDOUT_FILENO);
						dup(this->sockfd);
						execl(this->buf, this->buf, 0);
						exit(0);
					}
				}
			}
		}

	private:
		//c++98起允许在类内初始化静态整形编译期常量
		//c++17起支持内联变量
		static constexpr int BUFFER_SIZE = 1024;
		static int epollfd;
		int sockfd;
		sockaddr_in address;
		char buf[BUFFER_SIZE];
		//标记缓冲区已经读入的客户数据的最后一个字节的下一位置
		int read_index;
};


int main(int argc, char *argv[])
{
	if(argc <= 2){
		printf(" usage  ");
		return 1;
	}
	const char *ip = argv[1];
	int port = atoi(argv[2]);
	
	int listenfd = socket(PF_INET, SOCK_STREAM, 0);
	assert(listenfd >= 0);

	int ret = 0;
	struct sockaddr_in address;
	bzero(&address, sizeof(address));
	inet_pton(AF_INET, ip, &address.sin_addr);
	address.sin_port = htons(port);

	ret = bind(listenfd, (struct socckaddr *)&address, sizeof(address));
	assert(ret != -1);

	assert(listen(listenfd, 5) != -1);

	processpool<cgi_conn> *pool = processpool<cgi_conn>::create(listenfd);

	if(pool){
		pool->run();
		delete pool;
	}

	close(listenfd);

	return 0;
}

