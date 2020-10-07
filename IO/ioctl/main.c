/*************************************************************************
	> File Name: main.c
	> Author: ggboypc12138
	> Mail: lc1030244043@outlook.com 
	> Created Time: 2020年08月23日 星期日 17时19分36秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>

//int ioctl(int fd, int request, ...);

int main()
{
	struct winsize size;
	if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) < 0){
		perror("ioctl ");
		exit(1);
	}
	printf("%d rows, %d columns\n", size.ws_row, size.ws_col);
	return 0;
}


