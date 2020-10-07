/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年08月27日 星期四 18时00分58秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>

//1.管道(pipe)是半双工的
//2.管道只能在具有公共祖先的两个进程间使用
//int pipe(int fd[2]);
//经由参数fd返回两个文件描述符:fd[0]为读端,fd[1]为写端
//long fpathconf(int fd, int name);
//测试管道缓冲区大小，name取_PC_PIPE_BUF
int main(int argc, char *argv[])
{
	int fd[2];
	int pid;
	char buf[1024];
	if(pipe(fd) < 0){
		perror("create pipe fail");
		return 0;
	}
	if((pid = fork()) < 0 ){
		perror("fork fail");
		return 0;
	}
	if(pid == 0){
		close(fd[1]);
		int count = read(fd[0], buf, 1024);
		printf("...\n");
		write(STDOUT_FILENO, buf, count);
	}
	else if(pid > 0){
		close(fd[0]);
		sleep(3);
		write(fd[1], "hello pipe!!!", 13);
	}

	return 0;
}

//使用管道需要注意以下4种特殊情况（假设都是阻塞I/O操作，没有设置O_NONBLOCK标志）：
//1.如果所有指向管道写端的文件描述符都关闭了（管道写端的引用计数等于0），而仍然有进程从管道的读端读数据，那么管道中剩余的数据都被读取后，再次read会返回0，就像读到文件末尾一样。
//2.如果有指向管道写端的文件描述符没关闭（管道写端的引用计数大于0），而持有管道写端的进程也没有向管道中写数据，这时有进程从管道读端读数据，那么管道中剩余的数据都被读取后，再次read会阻塞，直到管道中有数据可读了才读取数据并返回。
//3.如果所有指向管道读端的文件描述符都关闭了（管道读端的引用计数等于0），这时有进程向管道的写端write，那么该进程会收到信号SIGPIPE，通常会导致进程异常终止。讲信号时会讲到怎样使SIGPIPE信号不终止进程。
//4.如果有指向管道读端的文件描述符没关闭（管道读端的引用计数大于0），而持有管道读端的进程也没有从管道中读数据，这时有进程向管道写端写数据，那么在管道被写满时再次write会阻塞，直到管道中有空位置了才写入数据并返回。
