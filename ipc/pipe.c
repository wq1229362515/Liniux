#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
//管道必须创建在创建子进程之前,因为我们必须来拿到这个文件操作符
//才可以来读取数据,不然的话怎么来读取数据呢????
//匿名管道 具有亲缘关系的进程之间的通信方式
/*
 * 进程之间通信的目的
 *
 * 1.数据的传输:一个进程需要将数据发送给另外一个进程
 * 2.资源的共享:多个进程之间共享资源
 * 3.通知事件的发生:一个进程需要向另外一个进程进行事件的通知,比如终止父子进程
 * 4.进程的控制:有些进程希望完全的控制另外一个进程的执行,
 *
 *
 * int pipe(int fd[2]);
 * 创建一个匿名管道
 * fd 文件描述符
 * fd[0]表示读端口
 * fd[1]表示写端口
 *
 * 0 读 1 写
 *
 * 成功返回0
 * 失败返回错误码
 *
 * 管道是unix中最古老的通信方式,匿名管道必须具有亲缘关系才可以读到数据
 * 进程创建了匿名管道之后,操作系统在内核中重建了一块缓冲区,并返回两个文件描述符
 * 作为操作句柄,来操做管道的读写,不过这个读写端是由我们用户自行来定义的,
 * 在内核的缓冲区中却是没有定义的
 * (本质就是内核中的缓冲区!)
 *
 * 当没有数据可以读的时候:
 * read函数是会阻塞的,当read调用阻塞的时候,进程暂停执行,一直等到有数据来为止
 * O_NONBLOCK(文件的状态标志) enable read 调用返回-1  errno值为 EAGAIN
 *
 * 当管道满的时候,wirte调用阻塞直到管道的对端来讲数据读取走
 * 管道的缓存大小 4096个字节,管道自带同步与互斥
 *
 *
 *
 *
 *
 * */

int  main (){


    /*
     *在进入进程之后,其实就可以更具各自的操作来关闭各自的读写端
      为了避免造成混乱

     * */
    int pipefd[2];
    int ret = pipe(pipefd);
    if(ret < 0){
        perror("pipe error");
        return -1;
    }
    int pid = fork();
    if(pid < 0){
        perror("fork error");
    }
    if(pid == 0){
        //子进程是读端
        //usleep(1000);
        char buf[1024];
        close(pipefd[1]);
        read(pipefd[0],buf,1023);
        printf("child [%s]\n",buf);
    }
    if(pid > 0 ){
        //父进程是写端
        close(pipefd[0]);
        printf("父端来写数据!\n");
        write(pipefd[1],"hello world",11);
    //size_t write(int fd, const void *buf, size_t count);

    }

    return 0;
}

/*=============================================================== 
*   Copyright (C) . All rights reserved.")
*   文件名称： 
*   创 建 者：
*   创建日期：
*   描    述：创建管道,将数据写到管道当中,再利用管道将数据读出 
*
* 1.创建管道
* 2.将stdin数据写入到pipe中去
* 3.读端读取管道的内容再写到stdout中去
================================================================*/

/*
int main(){



    char buf[1024] = {0};
    int pipefd[2];
    //int pipe(int pipefd[2]);
    int ret = pipe(pipefd);
    if(ret < 0){
        perror("pipe create errr");
        return -1;
    }
    printf("start!\n");
    //char *fgets(char *s, int size, FILE *stream);
    fgets(buf,10,stdin);
    size_t len = strlen(buf);
    //write(pipefd[1],"hello world",11);
    if(write(pipefd[1],buf,len) != len){
        perror("write error");
        return -1;
    }
    
    //将管道中的数据读出来
    //size_t len = ssize_t read(int fd, void *buf, size_t count);
    //memest(void *memset(void *s, int c, size_t n);)
    
    memset(buf,0,sizeof(buf));
    len = read(pipefd[0],buf,100);
    if(len < 0){
        perror("read pipe error");
        return -1;
    }
    write(1,buf,len);

    return 0;
}
*/




