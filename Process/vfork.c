#include<stdio.h>
#include <unistd.h>
#include<stdlib.h>
//
//vfork只能exit退出, 用return退出的话,因为共用一个函数栈,子进程都return了
//那么父进程在接到exit之后继续去从vfork那里去执行但是这时候函数栈已经
//被子进程干废了,那么就不能执行下去了
//vfork 和fork都会调用clone函数
//vfork 是fork的一个产物,因为之前许多子进程copy之后就干一个动作exce所以有点笨重
//所以vfork就运用而生了
//
//一个是share	vfork
//一个是copy   fork
//1>子进程先与父进程执行
//2>当子进程调用exit()或exec()后，父进程往下执行
int main(){

	int pid = vfork();
	if(pid < 0){
		printf("fork error\n");
	}
	else if(pid == 0){
		printf("i am child\n");
		sleep(1);
		//exit(0);
		return 0;
		//__exit(0);
	}
		printf("i am parent\n");
		sleep(1);
	return 0;
}
