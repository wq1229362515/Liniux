#include<stdio.h>
#include<unistd.h>
#include<error.h>
#include<stdlib.h>
int g_val = 100;
int main () {

    int pid  = fork();
    if(pid < 0 ){
        perror("fork error");
        return 0;
    }
    if(pid == 0){
        g_val = 0;
        printf("child: %d g_val = %d,&g_val = %p\n",getpid(),g_val,&g_val);
    }
    if(pid > 0){
        sleep(3);
        printf("parent : %d g_val = %d,&g_val = %p\n",getpid(),g_val,&g_val);
    }

    return 0;
}
