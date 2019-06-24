

//创建线程的demo
//
//进程就是线程组
//一个线程组里面有多个线程 ,所以线程是cpu调度的基本单位
//线程是一个轻量级的进程
//在传统的计算机系统中进程就是PCB
//但是在Linux中PCB就是线程,比传统的进程更加的轻量化
//
#include<stdio.h>
#include<pthread.h> //Linux中不提供线程的控制调用接口

//使用库函数创建的线程称为用户态线程,这个用户态线程在内核中使用一个轻量级进程实现调度

//用户态线程+轻量级进程
//进程是某个特定功能的程序和数据集合的一次活动,这个活动可能为了完成某个功能由用户或者系统主动
//发起.
void*  Test(void * arg){
    printf("tid :%p\n",pthread_self());
    while(1){
        printf("i am thread\n");
        sleep(1);
        //pthread_exit(NULL);
        int ret = pthread_cancel(pthread_self());
        if(ret != 0){
            perror("thread error");
        }
    }
}
int main (){
    //一 . 线程的创建
    //man 3  库函数调用手册
    //int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
    //                          void *(*start_routine) (void *), void *arg);
    //pthread_t  thread  返回线程的ID
    //attr  线程的属性 NULL表示默认属性
    //start_routine  :是一个函数的地址,线程启动后要执行的函数
    //arg  :  传给线程启动函数的参数 
    //返回值 成功返回0 ,失败返回错误码
    
    pthread_t tid;
    int ret;
    ret = pthread_create (&tid,NULL,Test,NULL);
    if(ret != 0){
        perror("thread error\n");
        return -1;
    }
    //线程ID代表的是线程在虚拟空间中的地址
    printf("tid :%p\n",tid);
    while(1){
        printf("i am main \n");
        sleep(1);
    }
    //二 .线程的终止 
    //线程中可以用void pthread_exit (void * value_ptr);
    //value_ptr 表示线程的返回值
    //无返回值 
    // int  pthread_cancel (pthread_t thread);
    // thread 就是线程的ID
    // 成功返回0,失败返回错误码
    return 0;
}
