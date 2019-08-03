#include"comm.h"



int main (){
    //1.创建内存
    int shmid = getshm(4096);
    //2.建立映射关系
    //将共享的内存连接到我们进程上,通过这个地址addr做桥梁
    //
    //void *shmat(int shmid, const void *shmaddr, int shmflg);
    //shmaddr  指定连接的地址  通常是置空的,让系统来为我们自己分配
    //
    char *addr = shmat(shmid,NULL,0);
    sleep(2);

    int i = 0;

    //执行相应的操作
    while(i < 26){
        addr[i] = 'A'+ i;
        i++;
        addr[i] = 0;
        sleep(1);
    }

    //执行解除关系操作
    shmdt(addr);
    sleep(2);


    return 0;
}
