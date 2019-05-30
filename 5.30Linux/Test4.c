#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<errno.h>

//非阻塞轮询查询方法
int main (){
	int status;
	while(waitpid(-1,&status,WNOHANG) == 0){
		// 阻塞等待任意一个子进程退出,获取返回值
		//waitpid(pid_t pid,int *status,int options)
		//pid = -1,等待任意一个子进程,与wait等效
		printf("drink offee\n");
		sleep(1);
	}
	if((status & 0x7f) == 0){
		printf("exit code : %d\n",(status >>8 )& 0xff);

	}
	while(1){
		printf("i am a student\n");
		sleep(1);
	}
	return 0;
}
