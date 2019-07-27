#include"TCPsocket.hpp"
#include<stdlib.h>


void * pthread_Func(void *arg){
    TcpSocket* clisock = (TcpSocket *)arg;       
    while(1){
        Calu data;
        int fd = clisock->GetFd();
        recv(fd, (char*)&data, sizeof(data), 0);
        cout<<"serv recv data:"<<data._num1<<data._Op<<data._num2<<endl;
    }
    clisock->Close();
    delete clisock;
    return NULL;
}
int main (int argc ,char* argv[]){
    if(argc != 3){
        cout<<"./Tsrv 192.168.247.128 9000"<<endl;
        return -1;
    }
    string _ip = argv[1];
    string _port = argv[2];
    TcpSocket serv;
    CHECK(serv.Socket()); 
    CHECK(serv.Bind(_ip,_port));
    CHECK(serv.Listen());
    //客户端接受到这个字符串的数据,然后需要反序列化,将其转换为结构体
    //然后显示接受到的信息
    while(1){
       pthread_t tid;           //声明一个线程
       TcpSocket* clisock = new TcpSocket();       //accept 接受到的套接字文件描述符

       int ret = serv.Accept(*clisock);
       if(ret < 0){
           cerr<<"serv accept error"<<endl;
       }
       //完成连接之后,就交给线程来进行任务的处理,主线程完成线程分离,等待其他的
       //套接字到来
       //int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
       //                          void *(*start_routine) (void *), void *arg);
       int ret1 = pthread_create(&tid,NULL,pthread_Func,(void* )clisock);
       if(ret1 != 0 ){
          cerr<<"pthread create error"<<endl;
       }
       pthread_detach(tid);
    }
}
