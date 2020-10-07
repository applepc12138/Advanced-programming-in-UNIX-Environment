/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年09月05日 星期六 21时10分39秒
 ************************************************************************/

#include <stdio.h>
#include <sys/socket.h>

// int socket(int domain, int type, int protocol);
// 返回值：成功返回文件描述符，失败返回-1
// domain:
//     AF_INET 这是大多数用来产生socket的协议，使用TCP或UDP来传输，用IPv4的地址
//     AF_INET6 与上面类似，不过是来用IPv6的地址
//     AF_UNIX 本地协议，使用在Unix和Linux系统上，一般都是当客户端和服务器在同一台及其上的时候使用
// type:
//     SOCK_STREAM 这个协议是按照顺序的、可靠的、数据完整的基于字节流的连接。这是一个使用最多的socket类型，这个socket是使用TCP来进行传输。
//     SOCK_DGRAM 这个协议是无连接的、固定长度的传输调用。该协议是不可靠的，使用UDP来进行它的连接。
//     SOCK_SEQPACKET 这个协议是双线路的、可靠的连接，发送固定长度的数据包进行传输。必须把这个包完整的接受才能进行读取。
//     SOCK_RAW 这个socket类型提供单一的网络访问，这个socket类型使用ICMP公共协议。（ping、traceroute使用该协议）
//     SOCK_RDM 这个类型是很少使用的，在大部分的操作系统上没有实现，它是提供给数据链路层使用，不保证数据包的顺序
// protocol:
// 0 默认协议
// 返回值：成功返回一个新的文件描述符，失败返回-1，设置errno


int main(int argc, char *argv[])
{
	
	return 0;
}
