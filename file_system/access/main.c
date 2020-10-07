/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年08月23日 星期日 20时00分30秒
 ************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if(argc < 2){
		printf("usage: access <pathname>");
		return 0;
	}
	if(access(argv[1], R_OK) == 0)
		printf("read file ok\n");
	else 
		printf("read permission denf\n");

	if(access(argv[1], W_OK) == 0)
		printf("write file ok\n");
	else 
		printf("write permission denf\n");

	if(access(argv[1], X_OK) == 0)
		printf("extcute file ok\n");
	else
		printf("extcute permission denf\n");
	return 0;
}


