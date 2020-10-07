/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年08月25日 星期二 09时38分59秒
 ************************************************************************/

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

//int mkdir(const char *pathname, mode_t mode);
//int mkdirat(int fd, const char *pathname, mode_t mode);

int main(int argc, char *argv[])
{
	if(argc == 1){
		printf("input argument too less\n");
		exit(1);
	}
	for(int i = 1; i < argc; ++i){
		if(mkdir(argv[i], S_IRWXU | S_IRWXG) != 0){
			perror("create dir fail");
			exit(1);
		}
	}
	return 0;
}

