#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>

int  main () {

    close (1);  // 标准输出流文件的关闭,是的printf不输出到标准输出流当中
    //但是又打开了一个新的文件,那么按照最小分配的原则, 新打开的文件描述符被
    //分配到1那个位置去了
    int fd = open ("./iostremTest.txt",O_CREAT|O_WRONLY,0664);
    if(fd < 0){
        perror("open error");
        return -1;
    }
    //一切皆文件,往标准输出的文件里面写数据
    //这是库函数的调用但是库函数的指针FILE * 是连接着的fileno ,由于fd改变了,但是他只是调用了接口,不用管抽走没抽走
    //
    close(1);
    FILE * myfile =  fopen("./iostremTest.txt","w");
    printf("fd: %d\n",fd);
    printf("fileno: %d\n",myfile->_fileno);
    //fflush()会强迫将缓冲区内的数据写回stream指定的文件中
    //不加上刷新缓冲区,不会输出到文件中去(因为是封装的库函数,所以自己带的缓冲区,要是不缓冲,
    //也给文件输出不去)
    //需要牢记的是这是一层封装
    //是一个库函数stdio.h中
    fflush(stdout);

    close (fd); 



    return 0;
}
