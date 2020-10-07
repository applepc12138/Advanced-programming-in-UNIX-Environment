/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年08月25日 星期二 12时07分13秒
 ************************************************************************/

#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if(argc != 2){
		printf("input on argument please\n");
		exit(1);
	}
	DIR *dirptr = opendir(argv[1]);
	if(dirptr == NULL){
		perror("open dir fail");
		exit(1);
	}
	struct dirent *dt = readdir(dirptr);
	while(dt){
		printf("%s\n", dt->d_name);		
		dt = readdir(dirptr);
	}
	return 0;
}

