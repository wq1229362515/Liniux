#include<stdio.h>

//获取环境变量
//
//argc 参数的个数也就是输入参数的个数 
//argv[]命令行参数,是可以被赋予的
//参数的第一个是我们程序运行时候的路径
//剩下的才是我们输入的参数信息
//我们在给出命令行参数的时候,argc参数的个数也就计算出了
//env[]是环境变量
int main (int argc,char *argv[],char *env[]) {

	int  i ;
	printf("%d\n",argc);
	for(i = 0;argv[i];i++){
		printf("argv[%d] = %s\n",i,argv[i]);
	}
	for(i = 0;env[i];i++){
		printf("env[%d] = %s\n",i,env[i]);
	}
	return 0;
}
