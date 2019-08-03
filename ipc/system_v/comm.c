#include"comm.h"



//将创建共享内存封装起来
//1.创建内存
//2.映射内存
//3.执行相应的操作
//4.解除映射
//5.删除内存
static int commshm(int size,int flag){

    //key_t ftok(const char *pathname, int proj_id);
    //
    //共享内存，消息队列，信号量它们三个都是找一个中间介质，来进行通信的，这种介质多的是。
    //就是怎么区分出来，就像唯一一个身份证来区分人一样。
    //你随便来一个就行，就是因为这。只要唯一就行，就想起来了文件的设备编号和节点，它是唯一的，
    //但是直接用它来作识别好像不太好，不过可以用它来产生一个号。ftok()就出场了。ftok函数具体形式如下：
    //ftok产生这个键值来产生唯一的序列号
    //int shmget(key_t key, size_t size, int shmflg);
    key_t key = ftok(PATHNAME,PROJ_ID);
    if(key < 0){
        perror("ftok error !");
        return -1;
    } 
    int shmid = 0;
    //key 这个共享段的名字
    //这个共享内存的大小
    //flag 权限标志位,他们的用法和mode模式一样
    //成功的话会返回一个非负数,是这个共享内存段的标识码
    //失败返回-1 
    if((shmid = shmget(key,size,flag)) < 0 ){
        perror("shmget error");
        return -2;
    }
    return shmid;
}

int destroyshm(shmid){
    //int shmctl(int shmid, int cmd, struct shmid_ds *buf);
    //shmid shmget 返回的内存标识码
    //cmd 将要采取的动作(有三个可取值)
    //buf 指向一个保存着共享内存的模式状态的数据结构
    //IPC_RMID 删除信号量
    if((shmctl(shmid,IPC_RMID ,NULL))< 0){
        perror("shmctl error ");
        return -3;
    }
    return 0;
}

/*
 * 服务端来创建共享内存
 * 客户端需要用创建共享内存来查看这个共享内存是否真的存在
 * 所以也是需要来调用这个函数的.
 * 
 *
 *
 *
 * */

int createshm(int size){
    // IPC_CREAT   如果共享内存不存在，则创建一个共享内存，否则打开操作。
    // IPC_EXCL     只有在共享内存不存在的时候，新的共享内存才建立，否则就产生错误。
    //如果单独使用IPC_CREAT，shmget()函数要么返回一个已经存在的共享内存的操作符，要么返回一个新建的共享内存的标识符。
    //如果将IPC_CREAT和IPC_EXCL标志一起使用，shmget()将返回一个新建的共享内存的标识符；如果该共享内存已存在，或者返回-1。
    //
    //IPC_EXEL标志本身并没有太大的意义，但是和IPC_CREAT标志一起使用可以用来保证所得的对象是新建的，而不是打开已有的对象。
    return commshm(size,IPC_CREAT | IPC_EXCL |0666);
}
int getshm(int size){
    return commshm(size,IPC_CREAT);
}
