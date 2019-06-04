#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main (){
	printf("---------");
	//printf("---------\n");  反斜杠也会刷新缓冲区
	sleep(2);
	//exit(0);  结束之前会刷新缓冲区  是一个函数   man 3
	//_exit(0); 不会刷新缓冲区  不是一个函数,是一个系统接口 man 2  缓冲区的数据会被丢弃
	return ;
}
