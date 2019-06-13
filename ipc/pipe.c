#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
//管道必须创建在创建子进程之前

int  main (){


    int pipefd[2];
    int ret = pipe(pipefd);
    if(pid < 0){
        perror("pipe error");
        return -1;
    }
    int pid = fork();
    if(pid < 0){
        preeor("fork error");
    }
    if(pid == 0){
        char buf[1024];
        read(pipefd[0],buf,1023);
        printf("child [%s]\n",buf);
    }
    if(pid > 0 ){
        write(pipefd[1],"hello world",1);


    }








    return 0;
}
