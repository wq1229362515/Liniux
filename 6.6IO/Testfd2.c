#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<error.h>
int main () {
    /*
    char buf[1024];
    //从标准输入里面读数据
    ssize_t s = read(0,buf,sizeof(buf));
    printf("%d\n",s);
    if(s > 0){

        buf[s] = 0;
        write(1,buf,strlen(buf));
        write(2,buf,strlen(buf));
    }
    */

    //int dup2(int oldfd,int newfd);
    /*char buf[1024] = "123456";
    int fd = open ("./iostremTest.txt",O_CREAT|O_RDWR);     //读写打开
    if( fd < 0 ){
        perror("open error");
        return -1;
    }
    //将标准输入的文件描述符换成fd ,也就是标准输这个文件换了,换成了原来fd描述符
    //所描述的文件,这样就指向了新文件,写到了新文件当中去
    dup2(fd,1);
    close(fd);
    ssize_t write_size = write(1,buf,sizeof(buf)-1);
    if(write_size < 0) {
        perror("read error");
        return -1;
    }
    printf("%s",buf);
    fflush(stdout);
    */




    umask(0);
    /*
    int fd1 = open ("fd1.txt",O_CREAT|O_RDWR,0664);
    int fd2 = open ("fd2.txt",O_CREAT|O_RDWR,0664);
    printf(" fd1 = %d \n fd2 = %d\n",fd1,fd2);
    //dup就是复制文件描述符,完成系统调用的功能
    //将第一个文件描述符复制到第二个文件描述符中去
    //关闭第一个文件描述符
    //这时候因为已经复制了
    //所以fd2就转换了,指向的是第一个文件
    //写fd2,就是写到了第一个文件去
    dup2(fd1,fd2);
    close(fd1);//关闭了文件描述符也就不存在了
    ssize_t ret = write(fd2,"fd1",3);
    if(ret < 0){
        perror("write error");
    }
    printf("fd1 :%d fd2 : %d\n");
    */
    
    
    int fd1 = open ("fd1.txt",O_CREAT|O_RDWR,0664);
    int fd2 = open ("fd2.txt",O_CREAT|O_RDWR,0664);
    printf("fd1 :%d\n",fd1);
    int ret = dup(fd1);
    if(ret < 0){
        perror("dup error");
    }
    printf("ret : %d fd1 :%d\n",ret,fd1);
    write(ret,"test",4);

    return 0;
}
