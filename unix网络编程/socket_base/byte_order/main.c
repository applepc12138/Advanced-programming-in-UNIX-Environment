/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年09月20日 星期日 19时34分31秒
 ************************************************************************/

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// uint32_t htonl(uint32_t hostlong);
// uint16_t htons(uint16_t hostshort);
// uint32_t ntohl(uint32_t netlong);
// uint16_t ntohs(uint16_t netshort);
// h表示host，n表示network，l表示32位长整数，s表示16位短整数
// 如果主机是小端字节序，这些函数将参数做相应的大小端转换然后返回，如果主机是大端字节序，这些函数不做转换，将参数原封不动地返回

int main(int argc, char *argv[])
{
	uint32_t i = htonl(1);
	printf("i = %d\n", i);
	uint16_t j = htons(1);
	printf("j = %d\n", j);

	uint32_t m = ntohl(1);
	printf("m = %d\n", m);
	uint16_t n = ntohs(1);
	printf("n = %d\n", n);
	return 0;
}

