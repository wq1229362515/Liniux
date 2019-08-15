/*=============================================================== *   Copyright (C) . All rights reserved.")
*   文件名称： 
*   创 建 者：
*   创建日期：
*   描    述： 
*               线程池类里面提供的功能:
*                   1.线程任务的出入队列操作(push,pop)
*                   2.线程池的退出
*                   3.线程任务的处理 其中任务来了实现的功能是要交给我们的用户来自定义实现内容
*                                    线程池的任务处理只是保证了其中的线程安全(同步+互斥),也就是说
*                                    只是处理了线程
*
*                   还有一个点是,线程的安全队列,这个队列保存的就是线程,这个类型就是线程任务类,是由
*                   我们自己来进行封装了,其中线程任务类里面有相关的数据和任务函数类型
*
                    线程池就是首先创建一些线程，它们的集合称为线程池。
                    使用线程池可以很好地提高性能，线程池在系统启动时即创建大量空闲的线程，程序将一个任务传给线程池，
                    线程池就会启动一条线程来执行这个任务，执行结束以后，该线程并不会死亡，
                    而是再次返回线程池中成为空闲状态，等待执行下一个任务。

                    多线程运行时间，系统不断的启动和关闭新线程，成本非常高，会过渡消耗系统资源，
                    以及过渡切换线程的危险，从而可能导致系统资源的崩溃。这时，线程池就是最好的选择了。


                    在我们线程池里面让线程处理任务的时候,就已经保证了调度线程的安全性,不会发生非线程安全的操作,只要
                    保证了线程有序的执行,线程池分配的任务,那么就可以合理的执行分配过来的任务

                    创建的是没有边界的线程池,但是我们里面的线程数量是一定的(这种线程是可以重用的)
*
*
*
*
================================================================*/
#include<queue>
#include<iostream>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
using namespace std;

#define Max_pthread 5
typedef bool (*handler_t)(int);
class Threadtask{
    public:
        Threadtask():_data(-1),_handler(NULL){}
        Threadtask(int data,handler_t handler):_data(data),_handler(handler) {
            cout<<pthread_self()<<" 线程任务构造完成--"<<endl;
        }
    private:
        int _data;
        handler_t _handler;

    public:
        void run() {
            _handler(_data);
        }
};

class Threadpool{

    public:
        Threadpool(int Max = Max_pthread):maxpth(Max),curpth(Max),_thr_quit(false){
            //int pthread_mutex_init(pthread_mutex_t *restrict mutex,
             //         const pthread_mutexattr_t *restrict attr);            
             pthread_mutex_init(&_mutex,NULL);
             /*
              *int pthread_cond_init(pthread_cond_t *restrict cond,
                            const pthread_condattr_t *restrict attr);
              * */
             pthread_cond_init(&_cond,NULL);
        }
        ~Threadpool() {
            //int pthread_cond_destroy(pthread_cond_t *cond);
            pthread_cond_destroy(&_cond);
            pthread_mutex_destroy(&_mutex);
        }

        /*
         * 线程池的初始化,也就是创建线程
         *
         * */
        bool PoolInit() {
            pthread_t tid;
            int i;
            for(i = 0;i < Max_pthread;++i){
                int ret = pthread_create(&tid,NULL,thr_star,this);
                if(ret < 0){
                    cout<<"pthread create error\n"<<endl;
                    return false;
                }
            }
            cout<<pthread_self() <<" 线程池初始化成功"<<endl;
        }

        bool Taskpop(Threadtask** task) {

            if(_task_queue.empty())
                return false;
            *task = _task_queue.front();
            _task_queue.pop();
            cout<<pthread_self() <<" 线程出线程池队列----"<<endl;
            return true;
        }
        /*
         *
         *在进行任务入队的时候,要保证队列现在只有一个任务在入队
         *保证这个线程池任务队列的安全性
          在执行完操作之后需要唤醒其他的在等待队列上面的线程(告知你们可以进行操作了)
         *
         * */
        bool Taskpush(Threadtask* task) {
            LockQueue();
            _task_queue.push(task);
            WakeALL();
            UnLockQueue();
            cout<<pthread_self()<<" 入队成功-------"<<endl;
            return true;
        }
        /*
         * 线程退出,如果线程数量> 0 ,那么就要唤醒线程,然后执行线程退出
         *
         * */
        bool Poolquit() {
            cout<<"线程池退出---\n";
            LockQueue();
            _thr_quit = true;
            UnLockQueue();
            while(curpth > 0) {
                WakeALL();
                usleep(1000);
            }
            return true;
        }

    private:
        queue<Threadtask*> _task_queue;  //线程的任务队列
        pthread_cond_t _cond;
        pthread_mutex_t _mutex;
        int maxpth;         //最大线程数
        int curpth;
        bool _thr_quit;



        /*
         * 线程池任务函数,处理的是线程的入队,然后处理线程的任务
         *
         * */
        static void *thr_star(void *arg) {
            Threadpool *task = (Threadpool *)arg;
            while(1) {
                cout<<"线程池处理线程的到来  -----"<<endl;
                task->LockQueue();
                //循环判断条件是否成立
                while(task->IsEmpty()) {
                    cout<<"队列为空等待线程进入线程池,进入线程等待---"<<endl;
                    task->Threadwait();
                }
                cout<<"队列不为空现在处理线程,线程出队"<<endl;
                Threadtask* tt;
                task->Taskpop(&tt);
                task->UnLockQueue();
                cout<<"线程已经出队列,即将执行对应的任务"<<endl;
                tt->run();
            }
        }

        void LockQueue() {
            /**
             *int pthread_mutex_lock(pthread_mutex_t *mutex);
             int pthread_mutex_trylock(pthread_mutex_t *mutex);
             int pthread_mutex_unlock(pthread_mutex_t *mutex);
             */
            pthread_mutex_lock(&_mutex);
        }

        void UnLockQueue() {
            pthread_mutex_unlock(&_mutex);
        }

        void WakeALL() {
            //int pthread_cond_broadcast(pthread_cond_t *cond);
            pthread_cond_broadcast(&_cond);
        }

        void WakeoneTask() {
            pthread_cond_signal(&_cond);
        }

        bool IsEmpty() {
            return _task_queue.empty();
        }

        void Threadwait() {
            if(_thr_quit) {
                cout<<"线程池标志位为真,执行线程退出"<<endl;
                Threadquit();
            }
            pthread_cond_wait(&_cond,&_mutex);
        }

        void Threadquit() {
            cout<<pthread_self()<<" 线程退出"<<endl;
            curpth--;
            UnLockQueue();
            pthread_exit(NULL);
        }
};

bool handler(int data) {
    cout<<"执行任务程序-------\n";
    srand(time(NULL));
    int n = rand()% 5;
    cout<<"Thread:"<<pthread_self()<<" Run:"<<data<<" sleep: " <<n;
    cout<<"执行完毕------------\n";
    sleep(n);
    return true;
}

int main () {

    Threadpool pool;
    int i;
    //初始化线程池
    pool.PoolInit();
    for(i = 0;i <10 ;++i) {
        //创建任务
        Threadtask* task = new Threadtask(i,handler);
        pool.Taskpush(task);       
    }
    pool.Poolquit();

    return 0;
}


