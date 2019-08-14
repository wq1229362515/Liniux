/*=============================================================== 
*   Copyright (C) . All rights reserved.
*   文件名称： 
*   创 建 者：
*   创建日期：
*   描    述： 基于环形队列的生产者消费者模型使用的是信号量 
*               环形队列的底层是数组
*
================================================================*/
#include<iostream>
#include<vector>
#include<semaphore.h>
#include<pthread.h>
#define Max 15

using namespace std;

class  Queue{

    public:
    /*
     * 1.信号量
     * 
     *
     *
     * */
        Queue(int capacity = Max):Ringqueue(capacity),cap(capacity){
            //int sem_init(sem_t *sem, int pshared, unsigned int value);
           sem_init(&data_sem,0,0);
           sem_init(&space_sem,0,cap);
           con_step = 0;
           pro_step = 0;
        }
        void putdata(const int& data) {
            //int sem_wait(sem_t *sem);
            sem_wait(&space_sem);
            Ringqueue[con_step++] = data;
            con_step %= cap;
            sem_post(&data_sem);
        }
        void getdata(const int& data) {
            sem_wait(&data_sem);
            Ringqueue[pro_step++] = data;
            pro_step %= cap;
            sem_post(&space_sem);
        }
        ~Queue(){
            sem_destroy(&data_sem);
            sem_destroy(&space_sem);
        }

    private:
       vector<int> Ringqueue;
       int cap;

       // int sem_destroy(sem_t *sem);
       sem_t data_sem;
       sem_t space_sem;

       int con_step;
       int pro_step;
};

/*
 *  实现生产者和消费者队列
 *  生产者push数据
 *  消费者pop数据
 *
 *
 *
 * */
void *p_pro(void* arg) {

    Queue *q =(Queue *) arg;
    int i = 0;
    while(1) {
        q->putdata(i++);
        cout<<pthread_self()<<"p_pro create data"<<i<<endl;
    }
}
void *c_con(void* arg) {
    Queue *q = (Queue *)arg;
    int i = 0;
    while(1) {
        q->getdata(i++);
        cout<<pthread_self()<<"c_con get data "<<i<<endl;
    }
}

int main (){


    pthread_t p_tid[4],c_tid[4];
    
    Queue q;

    int i;
    //p_tid
    for(i = 0;i<4;++i){
        int ret = pthread_create(&p_tid[i],NULL,p_pro,(void*)&q);
        if(ret != 0) {
            cout<<"thread create error"<<endl;
            return -1;
        }
    }
    for(i = 0;i<4;++i){
        int ret = pthread_create(&c_tid[i],NULL,c_con,(void*)&q);
        if(ret != 0) {
            cout<<"thread create error"<<endl;
            return -1;
        }
    }
    for(i = 0;i<4;++i) {
        pthread_join(c_tid[i],NULL);
    }
    for(i = 0;i<4;++i) {
        pthread_join(p_tid[i],NULL);
    }

    return 0;
}

