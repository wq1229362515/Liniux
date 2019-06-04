#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<errno.h>
//进程等待的问题

int main (){

	//wait 的方法 
	int pid = fork();
	if(pid < 0){
		printf("fork error :  %s\n",strerror(errno));
		perror("frok error\n");
	}
	else if(pid == 0){
	    //子进程睡眠三秒之后就会退出,如果不做任何提示的话,父进程不知道
		//就会造成僵尸进程的产生,僵尸进程Kill -9 也无能为力 ,因为谁也没办法杀死一个已经
		//死去的进程,造成了内存的泄露
		sleep (3);
	    exit(0);
	}
	//为了防止僵尸进程的产生我们应用了进程等待
	//就是父进程通过等待的方式,回收子进程的资源,获取子进程的退出信息
	//pid _t wait(status);
	//pid _t waitpid(pid ,status ,options);
	//
	//wait(int *status)
	//wait 函数的作用就是我一直等待子进程退出(等待任意一个子进程的退出即可)
	//阻塞等待 不关心进程退出状态的话就设置为NULL,返回的是子进程的pid,返回失败的话就是返回-1;
	//
	int ret = wait(NULL);
	while(1){
			printf("i am parent,child pid = %d\n",ret);
			sleep(1);
	}
	return 0;
}
