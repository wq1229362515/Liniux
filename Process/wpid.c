#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>

int main (){

	int pid = fork();

	if(pid < 0){
		printf("fork erron : %s\n",strerror(errno));
		perror("fork error");
	}
	if(pid == 0){

		sleep(3);
		//退出标志信息
		exit(11);
	}
	//一.关于进程的pid
	//pid 是上面定义的子进程id,所以用pid作为参数,不过我想一般的话是等待的是定义的子进程吧
	//这才是waitpid的意义,要么没有任何意义,和wait起到的效果是一样的
	//可以用-1 做参数等待任意一个子进程 ,和wait函数是等效的
	//第二个参数是返回的退出信息,不关心返回的信息的话就是NULL
	//pid_t waitpid(pid_t pid ,int *status,int options);
	//二.关于进程的操作参数
	//
	//Options 是WNOHANG
	//如果子进程没有结束那么就不等待了,那么waitpid返回的是0 ,那么可以利用他的返回值
	//做一个循环来判断是否退出与否,while 循环来做这个事情,
	//如果返回正常结束的话,和wait一样返回的是子进程的pid
	//waitpid(pid,NULL,WNOHANG);
	//
	//三.关于子进程的状态
	//
	//关于子进程的status 	
	//wait中的status是和exit函数有关系的
	//1>子进程退出时候利用exit定义的退出状态
	//2>父进程通过wait来获取该值从而分析
	//但是如果不关心进程退出的状况那么设置为NULL即可(假如只想回收资源的话)
	//进程有三种退出方式
	//1>代码运行完毕,结果正确
	//2>代码运行完毕,结果不正确
	//3>代码崩了
	//
	//
	/*
	while(waitpid(pid,NULL,WNOHANG) == 0){
		//非阻塞的轮询查询操作
	}*/
	int status;
	int ret; //子进程的pid记录
	//ret 必须写在进程等待这里,如果再给下面写进程等待就是错误的返回值了!
	while((ret = waitpid(pid,&status,WNOHANG)) == 0){

		//等待子进程结束
		printf("wait .....  child pid = %d\n",pid);
		sleep(1);
	}
	printf("%d\n",ret);
	if((ret > 0 && (status & 0x7f)) == 0){ //正常退出
		//ret 是返回的子进程的PId 
		//status的高16位没有存储任何数据
		//低16位的高八位存储的是返回值,低八位的最后一位存储的是core dump 核心转储 (程序异常退出保存程序信息)
		//低七位是0表示正常退出(没有异常退出(感觉比较拗口))
		//否则就是异常退出咯
		//显示退出码也就是高八位就要右移八位显示做个运算即可
		printf("child sign code :%d\n",(status >> 8) & 0xff);
	}
	//WIFEXITED(status) //若子进程正常退出为真(和上面的ret > 0 &&.... status的最后七位为0 等价)
	//WEXITSTATUS(status)//查看退出码
	//这是两个封装好的函数
	if(WIFEXITED(status)){
		printf("child sign code :%d\n",WEXITSTATUS(status));
	}
	else if(ret > 0){
		printf("sign code :%d\n",status&0x7f);	
	}
	while(1){
		printf("i am parent\n");
		sleep(1);
	}
	return 0;
}
