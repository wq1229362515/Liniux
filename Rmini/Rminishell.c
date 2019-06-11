#include<stdio.h>
#include<unistd.h>
#include<error.h>
#include <ctype.h>
#include<stdlib.h>
#include<fcntl.h>
int main (){
	while(1){
		printf("[miniShell @ local]:");
		//刷新缓冲区
		fflush(stdout);
		char temp[1024] = {"\0"};
        char *ptr = temp;
    	scanf("%[^\n]%*c",temp);
		printf("%s\n",temp);
        //实现重定向		
        // ls -l >> text.txt
        int flag = 0;               // 文件标志位
        char  *file_name = NULL;           //重定向文件名字记录
        while(*ptr) {
            //记录 >  符号的多少
            if(*ptr == '>'){
                *ptr = '\0';
                flag = 1;
                ptr++;
                if(*ptr == '>'){
                    *ptr = '\0';
                    flag = 2;
                    ptr++;
                }
                //记录完了>符号接下来就要记录一下文件名字了
                //如果空格就跳过
                while(isspace(*ptr) && *ptr !='\0' )ptr++;
                file_name = ptr;
                while(!isspace(*ptr) && *ptr !='\0')ptr++;
                *ptr = '\0';
            }
            ptr++;
        }
        
        int argc = 0;
		char *argv[32] = {"\0"};
		ptr = temp;

		while(*ptr){
		if(!isspace(*ptr)){
			argv[argc++] = ptr;
				while(!isspace(*ptr) && *ptr != '\0'){
					ptr++;		
				}
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
        umask(0);
        int pid = fork();
        if(pid < 0 ){
            perror("fork error");
        }
        else if(pid == 0){
            int fd  = 1; //文件描述符
            if(flag == 1 ){
                fd = open(file_name,O_CREAT|O_WRONLY|O_TRUNC,0664);
            }
            else if(flag == 2 ){
                fd = open(file_name,O_CREAT|O_WRONLY|O_APPEND,0664);
            }
            dup2(fd,1);
            //最后将argv最后一个赋值NULL了
            execvp(argv[0],argv);
            close(fd);
            //如果替换程序失败那么直接退出
            exit(0);
		}
		//父进程等待子进程退出
		wait(NULL);
	}
	return 0;
}
