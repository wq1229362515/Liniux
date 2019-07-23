#include <pthread.h>
#include"TCPsocket.hpp"
#include<stdlib.h>
#include <string.h>
void *pthreadserv (void *arg){
    TcpSocket* clisock = (TcpSocket *)arg;
    while(1){
        string data;
        clisock->Recv(data);
        cout<<"serv recv data:"<<data<<endl;;
        data.clear();
        cout<<"serv send data:";
        fflush(stdout);
        cin >> data;
        clisock->Send(data);
    }
    //线程之间共享描述符表
    //只能在普通线程在处理完关闭掉,不能在主线程中去关闭这个文件描述表
    //要不正在执行的线程就找不到文件描述表了
    clisock->Close();
    delete clisock;
    return NULL;
}
//线程版本服务器端,服务器在接受到连接请求之后,就在线程中去通信
int main (int argc ,char * argv[]){

    string ip;
    uint16_t port;
    stringstream temp;
    ip = argv[1];
    temp << argv[2];
    temp >> port;

    TcpSocket sock;
    CHECK(sock.Socket());
    CHECK(sock.Bind(ip,port));
    
    CHECK(sock.Listen()); 
    while(1){
        TcpSocket* clisock = new TcpSocket();
        sock.Accept(*clisock);
        //int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
       //                                   void *(*start_routine) (void *), void *arg);            
        pthread_t tid;
        int ret = pthread_create(&tid,NULL,pthreadserv,(void*)clisock);
        if(ret != 0){
            fprintf(stderr,"pthread_creat:%s\n",strerror(ret));
            exit(EXIT_FAILURE);
        }
        pthread_detach(tid);
    }
    sock.Close();
    return 0;
}

