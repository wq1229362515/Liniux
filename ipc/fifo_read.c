/*=============================================================== 
*   Copyright (C) . All rights reserved.")
*   文件名称： 
*   创 建 者：
*   创建日期：
*   描    述： 命名管道
================================================================*/

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<error.h>
#include<errno.h>
/*
 *
 *
 *  命名管道是一种特殊类型的文件
 *  如果不想在不相关的进程之间交换数据,可以使用FIFO文件来做
 *  这项工作,它经常被称为命名管道
 *
 *  命名管道可以从命令行上创建
 *  mkfifo filename(其实是一种文件的类型,创建好之后,文件的类型为p)
 *  
 * #include <sys/stat.h>
 * int mkfifo(const char *pathname, mode_t mode);
 *
 * 匿名管道是一种伪文件(本质就是内核中的缓冲区)
 * 匿名管道有pipe函数创建而成,
 * 命名管道mkfifo创建而成(是一种真实的文件,设备文件)在同一客户端的不同进程只要根据这个文件的路径就可以完成通信
 * FIFO(first input first output)
 *
 * 命名管道是通过网络来完成进程之间的通信的，命名管道依赖于底层网络接口，其中包括有 DNS 服务，TCP/IP 协
 * 议 等等机制，但是其屏蔽了底层的网络协议细节。
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 * */
//int mkfifo(const char *pathname ,mode_t mode);
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
    //匿名管道不用打开,是操作系统返回的描述符
    //但是命名管道是我们创建的
    //总之要有另外一个文件的模式与他想符合
    //文件没有被,是文件没被写的方式打开
    //文件没被已读的方式打开
    int fd = open (fifo,O_RDONLY);
    printf("start end -------------\n");
    if(fd < 0){
        perror("open error");
        return -1;
    }
    printf("fifo :%s open  success !!\n",fifo);
    //打开之后读数据
    //只读的方式打开
    while(1){
        char buf[1024] = {0};
        read(fd,buf,1023);
        printf("pear say: %s\n",buf);
    }
    close(fd);


    return 0;
}
