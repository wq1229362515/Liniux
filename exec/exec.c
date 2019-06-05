#include<stdio.h>
#include<unistd.h>


int main (int  argc, char *argv[],char *env[]){

    //替换函数 
	//调用exec并不创建进程,所以调用execl后程序进程id并没有发生改变
	//
	//int execl(const char *path ,const char *arg,....);
	//第一个参数是路径 要替换的程序的路径 ,第二个参数是参数选项(也就是程序的参数选项)
	//记得最后一个参数写完之后要加上NULL
	
	printf("这是一个程序替换范例:\n");

	//execl("/usr/bin/ls","ls","-a","-l",NULL);
	
	//带上p的话就是直接写程序名字即可
	//不带p要给上程序的路径,让系统自己去找程序
	//带p与不带p的区别就是路径的不同,先给出而写execlp的原型
	//int execlp (const char *path ,const char *arg,....);
	//经过试验这个NULL在后面不能加双引号,否则就失效喽
	//execlp ("pwd","pwd",NULL);
	
	//v和l的区别 :在于参数列表平铺还是数组形式的
	//int execv(const char *path,char *const argv[]);
	/*char *argv[32] = {"\0"};
	argv[0] = "ls";
	argv[1] = "-l";
	argv[2] = "-a";*/
	//不需要再后面加上NULL,还有给出指数组名即可
	//execv("/usr/bin/ls",argv);
	//带e和不带e的区别就是要运行的函数是否重新定义环境变量我们上面的五个默认的char *const argv[]是NULL
	//不需要重新定义环境变量,而且以上的的都是函数 在man3 中定义 
	//这个execve 是系统调用接口,直接和内核打交道
	//int execve(char *path ,char *const argv[],char *const envp[]);
	//第一个参数是路径,第二个参数是指定参数所以我们在最后要输入NULL作为结尾,第三个是环境变量
	//
	//
	//在这里运行了LS程序,将env给出的是NULL
	//int ret = execle("/usr/bin/ls","ls","-a","-l",NULL,NULL);
	//exec函数只有出错时候有返回值,而没有成功返回值
	//
	
	//运行的是我们自己写的程序
	//execle("./envi","ls","-a","-l",NULL,NULL);
	
	//运行加载了自己的环境变量
	//也就是说带e和不带e的区别就是要运行的程序是否需要重新定义环境变量(NULL表示默认的环境变量)
	execle("./envi","ls","-a",NULL,env);
	printf("这是一个程序替换范例\n");

	// 环境变量就是系统给我们搭建了程序的工作环境,让我们可以在想要的环境下去运行
	// 而不必在程序中繁琐的设置(我们也是可以去设置环境变量的)

	return 0;
}
