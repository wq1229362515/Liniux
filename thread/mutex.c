//线程间的互斥问题
//相关背景 
//临界资源:多线程执行流共享的资源就叫做临界资源
//临界区:   每个线程内部,访问临界资源的代码,就叫做临界区
//线程具有同步和互斥两种特性
//
//同步指的是对临界资源访问的时序合理性,比如在做面的时候,你得等面熟了再吃面
//不能面没有熟,你继续要面吃
//
//互斥的意思是访问的唯一性,在访问线程间共享的变量的时候,别的线程当然不能访问(这里的变量称为共享变量),
//否则就会造成安全问题,并发操作
//
//原子性: 不会被任何调度机制打断的操作,该操作只有两个状态,要么完成,要么未完成
//
//
//
//要解决代码中存在的这些问题 ,需要做到三点 
//1.代码要具有互斥行为,代码在进入临界区执行时候,不允许其他线程进入这个临界区域
//2.如果多个线程同时要求执行临界区的代码,并且临界区没有线程在执行,那么只允许一个线程进入
//3.如果线程不在临界区中执行,那么该线程不能阻止其他线程进入临界区
//
//本质上就是需要一把锁,Linux上提供这把锁叫 互斥量 
//
//
//互斥量 ,也就是加锁程序的5大步骤
//1.定义互斥锁变量
//1>静态分配:
//  pthread_mutex_t mutex = PTHREAD_MUTEX_INITITLIZE
//2>动态分配:
//  int pthread_mutex_init(pthread_mutex_t * restrict mutex,const pthread_mutexattr_t *restrict attr);
//  mutex  要初始化的互斥量
//  attr  NULL
//2.对互斥锁变量进行初始化
//3.对临界资源操作前先加锁
//4.对临界资源操作后要解锁
//5.销毁所有锁
//
//
//线程安全: 多个线程同时并发一段代码时候,不会出现不同的结果,常见对全局变量或者静态变量
//进行操作的时候,并且没有锁保护的情况下,会出现这个问题,因为有锁保护的话就不会并发执行同一段代码了
//
//
//可重入  同一个函数被不同的执行流执行,一个执行流在执行,另外一个执行流也进入执行,就是函数重入
//一个函数在重入了之后其线程的运行结果不会出现不同或者任何问题,那么就是可重入函数
//反之就是不可重入函数
//可重入与线程安全
//函数是可重入的,那么就是线程安全的
//线程安全不一定是可以重入的,可以重入的一定是线程安全的
//如果对临界资源加锁了,则这个函数线程是安全的,但如果这个重入函数若锁还未释放则会产生死锁,因此是不可以重入的
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

int  ticket = 100;
pthread_mutex_t mutex;   //定义一把锁
void *Test(void *arg){
    printf("%d 抢票中....   \n",(int)arg);
    while(1){
        pthread_mutex_lock(&mutex);  //3.加锁
        //if这个判断条件为真之后,代码可以切换到其他线程
        //在判断ticket的时候,就意味着进入到了临界资源的访问操作
        //所以在访问之前需要加锁,互斥锁 
        if(ticket > 0 ){
            
            //usleep 是模拟漫长业务的过程,在这个过程中也可能会有其他线程进来操作访问这个代码段
            usleep(100);
            printf("%d sells tickets ! %d\n",(int)arg,ticket);
            ticket--;
            //为什么票会减到-1 -2 因为在最后他们这个操作没有原子性,所以一个在减的时候,别的
            //也在并发执行,所以同时都认为自己抢到了票才执行--操作,这样的话就有了负数产生
            // -- 操作不是原子操作(不能被任何调度机制所打扰的机制)
            // load : 将共享变量ticket 从内存加载到寄存器
            // update : 更新寄存器里面的值,执行-1 操作
            // store : 将新值,从寄存器写回共享变量ticket的内存地址 
            //买票完成如果不解锁,那么就会造成死锁现象
            pthread_mutex_unlock(&mutex);  //4. 解锁
        }
        else{
            pthread_mutex_unlock(&mutex); //4.解锁 
            //在任何一个可能的出口我们都需要解锁
            //否则就会造成死锁的问题
            pthread_exit(NULL);            
        } 
    }
    return NULL;
}

int main (){
    int i = 0;
    pthread_t Y_Ball[4];
    pthread_mutex_init(&mutex,NULL);     //2.初始化锁
    for(i = 0;i<4;i++){
        int  ret = pthread_create(&Y_Ball[i],NULL,Test,(void*)i);
        if(ret != 0){
            fprintf(stderr,"pthread_create failed :",strerror(ret));
            return -1;
        }
    }
    for(i = 0;i<4;i++){
        pthread_join(Y_Ball[i],NULL);
    }
    //5. 销毁锁
    pthread_mutex_destroy(&mutex);
    return 0;
}
