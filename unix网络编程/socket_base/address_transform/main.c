/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年09月20日 星期日 20时48分43秒
 ************************************************************************/

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// 通用地址结构
// struct sockaddr;
// ipv4套接字地址结构
// struct sockaddr_in
// ipv6套接字地址结构
// struct sockaddr_in6
// const char * inet_ntop(int domain, const void *addr,
//	char * str, socklen_t size);
// 将网络字节序的二进制地址转换成文本字符串格式
// size指定保存文本字符串的缓冲区大小
// 返回值：若成功，返回地址字符串指针；若出错，返回NULL
//	int inet_pton(int domain, const char *str, void *addr);
// 将文本字符串格式转换成网络字节序的二进制地址
// 返回值：成功返回1；若格式无效，返回0；若出错，返回-1

int main(int argc, char* argv[])
{
	struct in_addr addr;
	//字符串转二进制格式
	int flag = inet_pton(AF_INET, "127.168.1.1", &addr.s_addr);
	printf("binary IP: %d\n", addr.s_addr);
	if(flag == -1)
		perror("transform fail\n");
	char str[32];	
	//二进制转字符串格式
	if(inet_ntop(AF_INET, &addr.s_addr, str, sizeof(str)))
		printf("str IP: %s\n", str);

	return 0;
}

