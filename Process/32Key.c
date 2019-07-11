#include<unistd.h>
#include<fcntl.h>

int main (){

    char str;
    int fd = open("/dev/s2",O_RDWR);
    if(fd  ==  -1 ){
        perror("open file error:");
        return -1;
    }
    int pid = fork();
    int fd;
    //子进程
    if(pid == 0){
        read(fd,&str,sizeof(str));
        printf("son process\n");
    }
    else if(pid > 0){
        read(fd,&str,sizeof(str));
        printf("father process\n");
    }
    else {
        perror("pid error\n");
        return -1;
    }

    return 0;
}
