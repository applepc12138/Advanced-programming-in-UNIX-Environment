/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年08月25日 星期二 16时27分29秒
 ************************************************************************/

#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
	DIR *dirptr = opendir("/home/ggboypc12138/c_project");
	if(dirptr == NULL){
		perror("open dir fail");
		return 0;
	}
	long c = telldir(dirptr);
	struct dirent *dt = readdir(dirptr);
	while(dt){
		printf("current location %ld\n", telldir(dirptr));
		printf("file name :%s\n", dt->d_name);
		dt = readdir(dirptr);
	}
	printf("-------------------------------\n");
	seekdir(dirptr, c);
	dt = readdir(dirptr);
	while(dt){
		printf("current location %ld\n", telldir(dirptr));
		printf("file name :%s\n", dt->d_name);
		dt = readdir(dirptr);
	}
	return 0;
}

