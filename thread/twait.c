#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>



void *thread1(void  *arg){
    printf("thread1 returning.....\n");
    printf("%p\n",pthread_self());
    int *p = (int *)malloc(sizeof(int));
    *p = 1;
    //强转为一个void 类型,因为要和函数类型相对应
    
    return (void *)p;
}

void *thread2(void  *arg){
    printf("thread1 exiting.....\n");
    printf("%p\n",pthread_self());
    int *p = (int *)malloc(sizeof(int));
    *p = 2;
    //强转为一个void 类型,因为要和函数类型相对应
    
    //eixt 和return  返回的内存单元必须是全局或者用那malloc分配的,不能在线程的函数栈上分配,因为
    //当其他线程得到这个指针时候线程函数就已经返回了
    //所以下面的返回给主线程的时候是没有意义的
    pthread_exit((void*)p);
}

void *thread3(void *arg){

    printf("thread3 running ............\n");
    printf("%p\n",pthread_self());
    while(1){
        printf("i am thread3\n");
        sleep(1);
    }
    return NULL;
}
void *thread4(void  *arg){
    printf("thread4 running.....\n");
    pthread_detach(pthread_self());
    printf("-----,%s\n",(char *)arg);

    return NULL;
}


int main(){

    //线程等待,创建三个线程,三个不同的退出原因
    //线程的等待是为了等待别的进程退出的原因,如果 退出了线程但是没有
    //释放空间就会造成资源的浪费,任然在地址空间里面保存着
    //创建新的线程不会去复用刚才的地址空间 
    //线程等待函数
    //pthread_t join  (pthread_t tid ,void **value_ptr);
    // 第一个参数 是线程ID
    // 第二个参数是   指向了一个指针,这个指针是在线程外所定义的,(二级指针),表面的意思就是
    // 指向了一个地址
    //
    //value_ptr  地址里面的内容
    //1.当线程是正常 return 返回的时候, ptr 所指向的单元就是thread的返回值
    //2.是别的线程异常调用cancle 终止的时候,存放的是PTHREAD_CANCELED常数
    //3.自己调用pthread_exit()的时候,存放的单元就是这个函数传给ptr的内容
    //4.终止状态不感兴趣的话,传给NULL即可

    pthread_t tid ;
    void *bet;
    int  ret;
    ret =  pthread_create(&tid,NULL,thread1,NULL);
    if(ret != 0){
        fprintf(stderr,"pthread_create error:%s\n",strerror(ret));
    }
    //在传值的时候,并没有直接传二级指针,像函数原型那样,而是取了一个一级指针的地址,也就是二级指针
    //在线程进程里面返回的是一个一级指针,现在要来接收这个一级指针(也就是接收这个地址),得用二级指针
    //也就是取一级指针的地址来接收这个返回值
    //void **val(初次来定义的时候代表二级指针,第二次 **val代表就是解引用就是一级指针指向的那个地址上的内容
    // *val 代表的是一级指针的地址  val代表是二级指针 
    // 指针本身是没有任何空间的,只是为什么有大小因为他地址的地址有大小,所以指针的空间存储的也就是地址
    //void *val(初次定义的时候代表一级指针,第二次 *val 代表的是解引用操作了)  &val  就是二级指针  
    //
    // 那么在这里直接用二级指针直接接收就有问题了,因为在线程返回值返回值是一级指针,
    //  void  **bet ;
    //  void *p;
    //  bet = p;
    //  这样是不行的
    //  只能 bet = &p;
    //或者 void *bet;
    //返回值 void *p;
    //&bet = p;
    //void **bet
    //
    //pthread_join(tid,&bet);
    //
    pthread_join(tid,&bet);
    // 等待指定的线程(tid)返回
    // tid 线程ID
    //  *(int *)bet  
    // 因为在join参数里面这个是一个void 类型的二级指针,要将返回值的code id  表现出来的话首先要进行强转
    // 换成int*类型的指针,因为就是一个指针,代表的也就是一个地址,这样转化是没有问题的
    printf("thread1 return ,thread id[%x],return code :%d\n",tid,*(int *)bet);
    free(bet);



    pthread_t tid2;
    pthread_create(&tid2,NULL,thread2,NULL);
    pthread_join(tid2,&bet);
    printf("thread2 return ,thread id[%x],return code :%d\n",tid2,*(int *)bet);
    free(bet);

    
    pthread_t tid3;
    pthread_create(&tid3,NULL,thread3,NULL);
    sleep(3);
    pthread_cancel(tid3);
    pthread_join(tid,&bet);
    if(bet == PTHREAD_CANCELED){
        printf("thread3 return ,thread id[%x],return code :PTHREAD_CANCELED\n",tid3); 
    }
    else{
        printf("thread3 return ,thread id[%x],return code :NULL\n",tid3);
    } 

    //线程分离
    //当我们不需要知道,不关心线程的返回值的时候这时候join就是一种负担
    //当我们只需要对线程的资源进行回收的时候,就可以使用线程分离函数进行减负担操作
    //所以我们需要进行修改默认的线程属性,默认的属性是join_able的
    //我们需要修改为detach属性
    pthread_t tid4;
    int ret1;
    ret1 = pthread_create(&tid4,NULL,thread4,NULL);
    if(ret1 != 0){
        fprintf(stderr,"pthread_create error  :%s\n",strerror(ret1));
        return -1;
    }
    int ret2 = 0;
    sleep(1);
    //线程创建好之后,进入线程完成线程分离,主线程需要等待一下
    //在分离好了之后,才可以等待,这样就不用去关心线程的返回值了,
    //也会自动释放内存资源
    //因为不关心线程的返回值,所以用NULL,但是并没有释放资源
    //所以要先等待一下线程pthread_detech 完成了
    //这样就不会线程等待了,只是针对不关心线程情况的条件下使用的
    if(pthread_join(tid4,NULL) ==  0){
        printf("wait success! ");
        ret2 = 0;    
    }
    else {
        printf("wait failed! ");
        ret2 = 1;
    }
    printf("ret2 = %d\n",ret2);
    return 0;
}
