/*生产者消费者模型:一个场所(互动的场所),两种角色(针对的是生产者和消费者),三种关系(保证了线程的安全性)
 *
 * 一个描述线程之间安全的进行互动的一个模型
 * 通过创建一个队列,间接的进行了互动的访问,这个队列可以看成为数据仓库
 * 这个数据仓库解决了耦合,闲忙不均与线程并发操作的问题
 * 
 * 要解决线程安全的问题就要知道三种关系
                   生产者和生产者的互斥
                    消费者和消费者的互斥
                    生产者和消费者的同步+互斥

如何实现:都是一个个线程,消费者/生产者线程
实现的是场所 :队列/线性表

首先要创建互斥锁,来保证互斥
创建消费者等待队列和生产者等待队列来实现同步,这样就保证了线程的安全
个类模型(这个类模型实例化的对象,就是消费者和生产者所共同能操作的)里面创建了一个队列来作为仓库,所有的操作写在类模型里面,进行操作
生产者进行放数据就叫做入队列,对入队列进行一层封装操作,因为如果直接放的话没有进行加锁互斥同步操作就造成了线程的不安全,

具体放的步骤(实现线程安全的步骤)

1.放数据之前先对操作加锁,判断队列是否满了,满了就等待消费者去取数据,同时等待在生产者队列上面
2.压栈数据
3.唤醒消费者队列上的线程
4.解锁操作

出队列:消费者进行取数据就叫做出队列,同样也出队列模型进行一层封装
1.取数据先加锁
2.判断是否为空,为空等待生产者产生数据,同时等在消费者队列上面
3.将数据取出给消费者,之后进行出队列操作,将仓库(缓冲区的数据弹出)
4.唤醒生产者队列上的线程
5.解锁操作


*/
#include<iostream>
#include<queue>
#include<pthread.h>
#include<unistd.h>

using namespace std;

//stl 容器都是非线程安全的

//因为队列的特殊性,要是一个对应一个就不需要上锁

#define Maxqueue 3
class BlockQueue{
    private:
    queue<int> _queue;
    pthread_mutex_t mutex;   //非线程安全的队列,加锁
    int capacity;           //最大节点数,如果一瞬间请求过来,
    pthread_cond_t cond_product;
    pthread_cond_t cond_consumer;
    
    public:
    BlockQueue(int _capacity = Maxqueue):capacity(_capacity){
        pthread_mutex_init(&mutex,NULL);
        pthread_cond_init(&cond_product,NULL);
        pthread_cond_init(&cond_consumer,NULL);
    }
    ~BlockQueue(){
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&cond_product);
        pthread_cond_destroy(&cond_consumer);
    }
    //入队是生产者进行入队操作,生产者将数据放入仓库
    bool pushData(int data){
        //为了防止此时,有别的线程对仓库进行操作要加锁操作
        pthread_mutex_lock(&mutex);
        //队列满了,就等待在product队列上面
            while(_queue.size() == capacity){
                pthread_cond_wait(&cond_product,&mutex);
        }
        _queue.push(data);
        //加入完数据之后,就可以唤醒消费者队列
        pthread_cond_signal(&cond_consumer);
        pthread_mutex_unlock(&mutex);
    }
    //  线程出队
    bool popData(int &data){
        pthread_mutex_lock(&mutex);
        //队列为0的话说明是没有数据的,那么就要生产者来产生数据 
        while(_queue.empty()){
            //等待在消费者队列
            pthread_cond_wait(&cond_consumer,&mutex);
        }
        //
        //能走下来说明有数据,因为是队列操作,那么就要先进先出,输出队头数据
        data = _queue.front();
        //弹栈操作
        _queue.pop();
        //能走下来说明有数据,唤醒生产者
        pthread_cond_signal(&cond_product);
        //唤醒消费者之后,解锁操作
        pthread_mutex_unlock(&mutex);
    }
};
//
//消费者线程函数是进行对队列取数据用的
void* F_con(void *arg){
    BlockQueue* q = (BlockQueue *)arg;
    while(1){
        int data;
        q->popData(data);
        cout<<pthread_self()<<"consumer get data"<<data<<endl;    
    }
    return NULL;
}
void* F_pro(void *arg){
    BlockQueue* q = (BlockQueue *)arg;
    int i = 0;
    while(1){
        q->pushData(i++); 
        cout<<pthread_self()<<"product put data"<<i<<endl;    
    }
    return NULL;
}

int  main() {

    //创建线程
    pthread_t con_tid[4],pro_tid[4];
    BlockQueue q;
    int i;
    for(i = 0;i<4;i++){
        int ret = pthread_create(&con_tid[i],NULL,F_con,(void *)&q);
        if(ret != 0){
            cout<<"thread create error"<<endl;
            return -1;
        }
    }
    for(i = 0;i<4;i++){
        int ret = pthread_create(&pro_tid[i],NULL,F_pro,(void *)&q);
        if(ret != 0){
            cout<<"thread create error"<<endl;
            return -1;
        }
    }
    //线程等待
    for(i = 0; i<4;i++){
        pthread_join(pro_tid[i],NULL);
    }
    for(i = 0;i<4;i++){
        pthread_join(con_tid[i],NULL);
    }
    return 0;
}















