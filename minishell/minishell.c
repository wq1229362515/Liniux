#include<stdio.h>
#include<unistd.h>
#include<error.h>
#include <ctype.h>
#include<stdlib.h>
int main (){


	while(1){
		printf("[miniShell @ local]:");
		//刷新缓冲区
		fflush(stdout);
		char temp[1024] = {"\0"};
		//%[^\n]正则表达式,从标准输入中取出数据值,遇见\n才截止
		//%*c 从缓冲区里面取出一个字符丢掉
		//在这个程序里面,利用正则表达式取出了除了最后一个\n之前的所有数据
		scanf("%[^\n]%*c",temp);
		printf("%s\n",temp);
		
		//取出之后就可以创建子进程le

		int pid = fork();
		if(pid < 0){
			printf("fork error :%s",strerror(error));
		}
		if(pid == 0){

		//进入子进程里面,进行程序替换
		//替换的就是temp里面的数据
		//execlp(temp,temp,NULL);
		//但是按照这样的方式替换的话 la -a -l  等后面的参数就没办法执行了
		//这时候就需要定义一个字符数组指针,来存储,那么也就不能用execlp这个函数了
		//也就要用execvp函数

		//首先要定义几个指针数据
		int argc = 0;
		char *argv[32] = {"\0"};
		char *ptr = temp;

		while(*ptr){
		//如果ptr 没有遍历到temp字符数组的最后一个,那么就继续遍历
			//如果不是空白字符那么进入块中来对argv数组赋值
			if(!isspace(*ptr)){
				//将地址赋予给argv
				//当遇到不是空白字符的时候
				//记录了地址之后,那么ptr需要寻找下一个参数了
				//因为我们argv的一个元素就存储一个参数地址的开头
				argv[argc++] = ptr;
				//下一个是空格就跳出
				while(!isspace(*ptr) && *ptr != '\0'){
					ptr++;		
				}
				//这时候ptr是空格了那么置为'\0'
				//这样就结束了一个argv元素的存储
				*ptr = '\0';
				ptr++;
				continue;
			}
		ptr++;	
		}
		if(!(strcmp(argv[0],"cd"))){
			//内建命令,遇到cd命令的时候
			//改变当前的工作路径
			chdir(argv[1]);
			continue;
		}
		//最后将argv最后一个赋值NULL了
		argv[argc] = NULL;
		execvp(argv[0],argv);
		//如果替换程序失败那么直接退出
		exit(0);

		}
		//父进程等待子进程退出
		wait(NULL);
	}
	return 0;
}
