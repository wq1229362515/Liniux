#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main (){

	pid_t pid;
	printf("Before :pid is %d \n",getpid());
	if((pid  =fork()) == -1){
		perror("fork()");
		exit(1);
	}
	//父进程返回子进程Pid
	//子进程返回0
	printf("After:pid is %d,fork return %d \n",getpid(),pid);
	sleep(1);
	return 0;

}
