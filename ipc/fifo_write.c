/*=============================================================== 
*   Copyright (C) . All rights reserved.")
*   文件名称： 
*   创 建 者：zhang
*   创建日期：
*   描    述： 命名管道
================================================================*/

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<error.h>
#include<errno.h>
#include<string.h>
//int mkfifo(const char *pathname ,mode_t mode);
//创建一个管道,管道只是内核中的缓冲区
int main(){

    char *fifo = "./test.fifo";
    umask(0);
    int ret  = mkfifo(fifo,0664);
    //如果文件存在了那么不应该退出
    if(ret < 0){
        if(errno != EEXIST){
            perror("mkfifo error");
            return -1;
        }
    }
    printf("start open --------------\n");
    //接下来操作管道文件
    //会卡在open这里
    //匿名管道不用打开,操作系统返回描述符
    //但是命名管道是我们创建的
    //总之要有另外一个文件的模式与他想符合
    //文件没有被,是文件没被写的方式打开
    //文件没被已读的方式打开
    int fd = open (fifo,O_WRONLY);
    printf("start end -------------\n");
    if(fd < 0){
        perror("open error");
        return -1;
    }
    printf("fifo :%s open  success !!\n",fifo);
    //写数据
    while(1){
        char buf [1024] = {0};
        printf("i say :");
        fflush(stdout);
        scanf("%s",buf);
        //向管道中写入数据,只要这边一些那边的进程一读取就会完成相应的操作
        write(fd,buf,strlen(buf));
    }
    close (fd);

    return 0;
}
