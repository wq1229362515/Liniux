#include<stdio.h>
#include<unistd.h>
#include<error.h>


int main (){

    //ls |grep make

    int pipefd[2];
    int ret = pipe(pipefd);

    int pid1 = fork();
    if(pid == 0){
        //ls
        clsoe(pipefd[0]);
        dup2(pipdfd[1],1);
        execlp("ls","ls",NULL);
        exit(0);
    }
    int pid2 = fork():
    if(pid2 == 0){
        //关闭所有写端
        close(pipefd[1]);
        dup2(pipefd[2],0);
        execlp("grep","grep","make",NULL);
        exit(0);
    }
    close(pipefd[1]);
    close(pipefd[0]);
    waitpid(pid1,NULL,0);
    waitpid(pid2,NULL,0);

    return 0;
}
