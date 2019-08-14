
//线程的同步 :线程的时序合理性  
//等待 +  唤醒 +等待队列 
//
//对于线程来说,我们假设要实现某一个动作需要一个前提条件,我们去面馆前提得有一碗面
// 那么如果现在不合适操作(没有面)就等待
//如果满足条件(有面)操作了就唤醒操作
//如果不满足条件我们就需要执行等待操作
//
//条件变量实现同步 等待+唤醒+等待队列
//条件变量只是提供了这些功能,但是具体什么时候等待(线程)什么时候唤醒,需要用户自身来判断
//也就是进程自身来判断
//
//同步指的是在保证线程安全的情况下,让线程能够按照特定的顺序访问临界的资源,
//从而避免了饥饿问题
//
//条件变量是用来等待线程而不是上锁的.条件变量通常和互斥锁一起用,因为上锁有两种状态
//锁定和非锁定,条件变量可以通过允许线程阻塞和等待另一个人线程发送信号来弥补互斥的的不足
//当条件不满足的时候允许一个执行流挂起和等待,要判断条件及需要进行访问临界资源,那么为了保证互斥性
//就需要加互斥锁进行保护
//
//条件变量的实现:
//
//1.定义条件变量  
//2.初始化
//3.条件变量提供等待功能 pthread_cond_init(&cond,&mutex)/pthread_cond_timedwait()
//  为什么要和互斥锁一起使用?
//      因为操作临界资源了之后,假如我们的条件没有成立,那么就要加载到等待队列里面,条件变量的条件等待实现了三步操作,解锁,等待,被唤醒重新加锁
//      线程等待那么就要有等待的条件,所以就要访问临界资源,要处理数据,就要进行判断??
//      这时候访问临界资源了,就要和互斥锁一起使用了,临界资源操作需要受保护(互斥锁,这是默认的)
//
//  有没有面,
//  没有面则等待 (这个临界操作就需要受保护,加锁,操作之前)
//  这时候判断了如果是等待就要解锁了,
//4.其他线程促使条件满足之后,唤醒等待队列上的线程
//
//pthread_cond_signal(&cond); ---唤醒至少一个线程
//pthread_cond_broadcast(&cond);--
//5.销毁条件变量
//
//实现一个顾客一个厨师
//实现多个顾客多个厨师
#include<pthread.h>
#include<stdio.h>

int noodle = 0;  //默认没有面
pthread_mutex_t mutex;
pthread_cond_t cond;
pthread_cond_t cond_eat;
pthread_cond_t cond_cook; //
//假设吃面的人先进来,加锁判断,没有面,陷入等待
//这个时候厨师加锁就进不来了
//所以休眠之前先解锁
void *eat_noodle(void *arg){
    while(1){
        //有面才能吃面,没有面
        //临界资源操作 加锁
        pthread_mutex_lock(&mutex);
        //while(noodle == 0){
        while(noodle == 0){
            //continue 没有休眠CPU占用率非常高,为了等待耗费cpu不划算
            //所以休眠
            //top查看CPU资源
            //usleep(1000);
            //continue;
            //不用continue,不安全
            
            //等待着,做好面,唤醒等待的人
            
            //假设吃面的人先进来,加锁判断,没有面,陷入等待
            //这个时候厨师加锁就进不来了
            //1.所以休眠之前先解锁
            //2.陷入休眠
            //3.被唤醒之后再加锁,要吃面了
            //现在刚解锁,没来得及休眠
            //这时候做面的人过来了
            //唤醒了吃面的人,
            //但是这时候,还没有陷入休眠
            //白唤醒了一次
            //厨师不会二次唤醒
            //双方陷入等待
            //
            //没有面等待之前要解锁,否则厨师做不了面
            //解锁之后,还没有睡眠,这时候,厨师刚好做了一碗面,
            //唤醒PCB_taskrunning状态
            //但是这时候,就是taskrunning状态,还没睡呢,所以没法唤醒,就白费了!!!!但是不会唤醒第二次,所以就死等了
            //错误的情况很容易碰上
            //
            //
            //
            /*pthread_mutex_unlock(&mutex);
            //解锁和休眠是原子操作,不可被打断,等休眠之后才可以被打断
            pause();
            pthread_mutex_lock(&mutex);*/
            //被唤醒加锁吃面

            //这个操作是一个阻塞操作
            //k:
            pthread_cond_wait(&cond_eat,&mutex);    //集合了三步操作 解锁 + 等待队列 + 加锁
            
        }
        //能走下来说明有面了 noodle == 1;
        printf("顾客吃面了!\n");
        //吃完了刷新资源
        noodle = 0;
        //吃完了之后,在等待下一碗
        //唤醒厨师
        pthread_cond_signal(&cond_cook);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *make_noodle(void* arg){
    //多个厨师,没有面做一碗面,唤醒吃面的人
    //唤醒顾客,至少唤醒一个或者多个,
    //这时候有人抢到时间片,加锁
    //对于其他顾客来说加不上锁,这时候不是cond的等待,是锁等待,因为都被唤醒了
    //挂到锁上面,第一个顾客吃完了,解锁操作,唤醒厨师做面,唤醒等待的厨师,
    //因为解锁了,谁都能抢,唤醒所有想要加锁的人,厨师和顾客都想加锁,
    //这时候很有可能抢到锁的是顾客,程序是顺序进行的,没有进行二次判断,这时候没有面
    //反过来,厨师也是一样的,所以if语句换成while
    //所以条件变量应该循环判断
    //
    //假若一个顾客进来了,吃面,这时候没有面,等待
    //厨师进来了,做面,其他厨师等待,面做好了,唤醒,唤醒厨师或者顾客
    //但是可能唤醒的可能是厨师,这时候另外一个厨师判断有面然后等待然后就卡死了,
    //
    //厨师和顾客等到一块去了,他们等待的地方应该不一样
    while(1){
        //有面没人吃就不做面
        pthread_mutex_lock(&mutex);
        while(noodle == 1){
           // usleep(1000);
           // continue;
           //等待吃面的人
           pthread_cond_wait(&cond_cook,&mutex);
        }
        printf("厨师做了面!\n");
        noodle = 1;
        //做好面之后要唤醒吃面的人
        //条件变量的唤醒函数
        //唤醒吃面的人
        pthread_cond_signal(&cond_eat);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
int  main(){



    pthread_t tid1,tid2;
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond_eat,NULL);
    pthread_cond_init(&cond_cook,NULL);

    int i;
    for(i = 0;i<3;i++){
        int ret = pthread_create(&tid1,NULL,eat_noodle,NULL);
        if(ret != 0 ){
            printf("pthread creat error!\n");
            return -1;
        }
    }
    for(i = 0;i<4;i++){
        int ret = pthread_create(&tid2,NULL,make_noodle,NULL);
        if(ret != 0 ){
            printf("pthread creat error!\n");
            return -1;
        }
    }

    //不想让主线程退出,所以等
    //线程等待
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_eat);
    pthread_cond_destroy(&cond_cook);

    return 0;
}
