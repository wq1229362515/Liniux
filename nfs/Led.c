#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<error.h>
int main(int argc, const char *argv[])
{
	int i;
	int fd[4];
	const char *str[] = {"/dev/myled1","/dev/myled2","/dev/myled3","/dev/myled4"};
	const char *ledon[] = {"led1on","led2on","led3on","led4on"};
	const char *ledoff[] = {"led1off","led2off","led3off","led4off"};
	for(i = 0;i<4 ;i++){
		fd[i] = open(str[i],O_RDWR);
		if(fd[i] == -1) {
			printf("fd[%d]\n error",i);
			perror("open error fd");
			return -1;
		}
	}
	while(1){
		for(i = 0;i<4;i++){
			write(fd[i],ledon[i],6);
			sleep(1);
			write(fd[i],ledoff[i],7);
			sleep(1);
		}
	}
	for(i = 0;i<4;i++){
		close(fd[i]);
	}
	return 0;
}
