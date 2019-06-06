#include<stdio.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>
//filecontrl(文件控制)
//man 2 系统调用接口函数
//man 3 封装函数(常见的C函数)
//  int open(const char *pathname, int flags, mode_t mode);
//  ssize_t read(int fd, void *buf, size_t count);
//  ssize_t write(int fd, const void *buf, size_t count);
//  off_t lseek(int fd, off_t offset, int whence);
//  int close(int fd);
//
//
//


int main (){
//  一.open函数 man 2 open 
//  open 函数有三个
//   int open(const char *pathname, int flags);     打开文件,不能创建,打开的是已经创建好的文件
//   int open(const char *pathname, int flags, mode_t mode)     mode_t 是用户的权限设置,用于创建文件
//   int creat(const char *pathname, mode_t mode);          创建一个文件,但是没有后续操作,因为没有操作参数
//  是有返回值的打开成功的话 ,返回的是fd 文件描述符,失败返回-1
//  函数第一个pathname表示要打开的文件名,或者要创建的文件
//  flags :打开文件时候,可以传入多个选项参数
//  O_RDONLY    只读打开
//  O_WRONLY    只写打开
//  O_RDWR      读写打开
//
//  //上面三个选项是必选项也就是必定指定一个
//
//  O_CREAT     若文件不存在则创建他,需要使用mode选项,来指明创建文件的权限
//  O_APPEND    追加写的方式
//  O_TRUNC         截断文件 下次打开的时候截断文件长度,长度为0

    umask(0);
  
   int fd = open ("./iostremTest.txt",O_RDWR|O_CREAT|O_APPEND,0664);
   if(fd < 1){
        perror("open perror");
        return -1;
   }
   
    //写文件操作
    //ssize_t write(int fd, const void *buf, size_t count);
    //fd 文件描述符
    //buf 要写的内容
    //count 要写的长度
    //要写的文件是和上面打开的方式有关系的
    //上面用的是追加的方式来打开的
    char buf[1024] = {'\0'};
    ssize_t ret = write(fd,"ni hao ",4);
    if(ret < 0){
        perror("open perror");
            return -1;
    }


//  off_t lseek(int fd, off_t offset, int whence);
   //  lseek 移动文件指针 
   //  fd  文件描述符
   //  whence 文件移动参考点    SEEK_SET 文件的开头    SEEK_END 文件的位置    SEEK_CUR   当前的位置
   //  offset 文件偏移量     
   //
   //从文件的开头开始算
    //在写完文件之后要读的话就把文件指针放到想要的位置
    lseek(fd,0,SEEK_SET);



   //  读文件函数 
   //  ssize_t read(int fd, void *buf, size_t count);
   //  fd  是 open函数所返回的文件描述符 
   //  buf 是要读写到指定的位置处,count是要读取的字节数
   
   ret = read(fd,buf,1023);
   if(ret < 0){
        perror("read error");
        return -1;
   }
   printf("%s\n",buf);

   //f类函数是这些系统调用函数的封装
   //f类函数有文件流描述符
   //文件描述符就是一个小整数
   //操作结束之后要关闭文件,要么造成了资源浪费
   // int  close(fd);
    close(fd);
    return 0;
}
