#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/stat.h>
//
// 一.
//  子进程先于父进程退出,但是并没有产生状态信息让父进程接受,这就是僵尸进程
// 二
//  父进程先于子进程退出,但是子进程并没有退出,那么子进程被一号进程领养,称为孤儿进程
// 三.
//  守护进程(精灵进程)是一类在后台运行的特殊进程,用于执行特定的任务,很多守护进程在系统引导的时候启动
// 并且一直运行到系统关闭,另一些只在需要的时候才启动,完成任务就自动结束
// 不受用户登录与注销的影响,他一直运行着,许多LInux下的大多数服务器都是利用守护进程实现的
// 可能在你需要的时候守护进程才会出现
// 守护进程脱离了终端界面是独立的在后台运行
// ps axj |more 查看所有用户作业
// ps axj | grep xxxx 查看守护进程
// 守护进程的信息
// TPGID = -1;
// pgid  进程组id
// sid 几个进程组可以合并成一个会话组（使用setsid系统调用）
// 可以用于终端程序设计。会话组中所有进程都有相同的SID
void Creatfork(){

    /*
    int pid = fork();
    if(pid < 0 ){
       perror("open error") ;
    }
    if(pid  ==  0){
        printf("i am child\n");
        //僵尸进程
        sleep(3);
        
        //模拟守护进程
        //sleep(15);
        exit(0);
    }
    else if(pid > 0){
        while(1){
             printf("i am parent \n");
             sleep(1);


             //模拟守护进程
             //sleep(5);
            // exit(0);
        }
    } */

    //这里第一次fork的作用就是让shell认为这条命令已经终止，不用挂在终端输入上;
    //再一个是为了后面的setsid服务，因为调用setsid函数的进程不能是进程组组长(会报错Operation not permitted)
    //如果不fork子进程，那么此时的父进程是进程组组长，无法调用setsid。所以到这里子进程便成为了一个新会话组的组长。
    int pid  = fork();
    if(pid < 0 ){

        perror("fork error");
    }
    if(pid > 0 ){
        exit(0);
    }
    else if( pid == 0 ){
        setsid();   //创建一个新的会话
        signal(SIGCHLD,SIG_IGN);
        umask(0);   //改变子进程的掩码,因为可能我们需要不同的权限
       
       
        //第二次fork：第二次fork是为了避免后期进程误操作而再次打开终端。
        //因为打开一个控制终端的前提条件是该进程必须为会话组组长
        //而我们通过第二次fork，确保了第二次fork出来的子进程不会是会话组组长。
       
       
        pid = fork(); 
        if(pid != 0){ 
            exit(0);
        }
        //将当前目录更改为根目录
        //当守护进程当前工作目录在一个装配文件系统时
        //该文件系统不能被拆卸。一般需要将工作目录改为根目录。
        chdir("/");     


        //关闭文件描述符,因为子进程继承了父进程的文件,但是可能用不到,所以关闭
        //这个函数是用来返回文件打开项数的,进程的文件数目
        //Get the size of the file descriptor table
        int maxfd = getdtablesize();
        while(maxfd -- ){
            close(maxfd);
        }
    }
}

int main (){

    Creatfork();
    while(1){
        sleep(1);
    }

    return 0;
}
