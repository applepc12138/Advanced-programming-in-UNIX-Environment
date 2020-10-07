#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

//会话session是一个或多个进程组的集合
//pid_t setsid();
//仅非组长进程可调用setsid()
//调用此函数的进程不是一个进程组的组长时,此函数创建一个新会话
//具体来说:
//1.该进程成为新会话的会话首进程
//2.该进程成为一个新进程组的组长进程,新进程组id为该进程的进程id
//3.该进程没有控制终端,若在调用setsid之前该进程有一个控制终端,那么此关系被切断

//pid_t getsid(pid_t pid);
//成功返回会话首进程的进程组id,失败返回-1

int main(void)
{
	pid_t pid;
	if ((pid = fork())<0) {
		perror("fork");
		exit(1);
	} else if (pid == 0) {
		printf("child process PID is %d\n", getpid());
		printf("Group ID of child is %d\n", getpgid(0));
		printf("Session ID of child is %d\n", getsid(0));
		sleep(10);
		setsid(); // 子进程非组长进程，故其成为新会话首进程，且成为组长进程。该进程组id即为会话进程
		printf("Changed:\n");
		printf("child process PID is %d\n", getpid());
		printf("Group ID of child is %d\n", getpgid(0));
		printf("Session ID of child is %d\n", getsid(0));
		sleep(20);
		exit(0);
	}
	return 0;
}




