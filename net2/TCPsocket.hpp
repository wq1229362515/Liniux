#pragma once

#include<iostream>
#include<sys/socket.h>
#include<sstream>
#include<string>
#include<stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<pthread.h>
using namespace std;
/* 封装TCP套接字
 *
 *
 * TCP连接通信的几个步骤 
 *
 * 1.创建TCP套接字socket
 * 2.为套接字绑定地址信息bind
 * 3.服务器端开始监听 Listen
 * 4.服务器端为客户建立新的socket accept()
 * 
 *
 * 5.send
 * 6.recv 
 *
 *
 *
 * 
 *  关于网络的协议,就像嵌入式那个模块一样,虽然他们之间没有用到网络,也不能说没有用到网络
 *  只是他们之间的通信只是单机的版本,不涉及到网络之间通信
 *
 *  但是进行了串口之间的通信,串口之间通信从上位机传输下来的数据上位机跟着接受
 *  就要根据人家的载波模块的协议来解析
 *  而解析这个协议的程序是需要工程师自己来完成的
 *
 *  只要进行了网络端口之间的通信那么就牵扯到了相关的网络传输,根据七层模型或者五层模型
 *  总之之间的通信总是要用到协议来进行解析的,
 *  我们要完成的大多是应用层的程序,解决我们的实际问题,那么如果没有适合的传输协议,那么我们就需要自己写
 *  一种通信协议来完成从一端到另外一端的接收了,总之我们要完成的协议必须具备这种完成通信的能力.
 *
 *  实现一个网络版本的加法计算器
 *  1.我们可以发送字符串来让服务器来解析这个字符串,然后处理相关的操作
 *  2.我们可以定义一个结构体来完成这些数据的存储,很显然第二种的稍微能好处理一点
 *    发送数据的时候,是一个字节一个字节的发送,那么意味着我们得把这个结构体转换为字符串,接受到数据的时候再按照
 *    相同的规则把字符串转换回这个结构体
 *    这个过程为序列和反序列化
 *
 *
 *
 *
 * */
class Calu{
    public:
        int _num1;
        int _num2;
        char _Op;
    public:
        Calu(){}
        Calu(int num1,int num2,char Op):
            _num1(num1)
            ,_num2(num2)
            ,_Op(Op)
            {}
};


#define CHECK(q){if ((q) == false) return -1;}
#define Max_backlog 5
class TcpSocket {
    public:
        TcpSocket():_fd(-1){
    }
        uint16_t change_int(const string&  port){
            uint16_t _port;
            stringstream temp;
            temp << port;
            temp >> _port;
            return _port;
        }

        bool Socket(){
            //int socket(int domain, int type, int protocol);
            _fd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
            if(_fd < 0){
                cerr<<"TCP socket error!"<<endl;
                return false;
            }
            return true;
        }
        void Close(){
            close(_fd);
        }
        bool Bind(const string _ip,const string  _port){
            //int bind(int sockfd, struct sockaddr *my_addr, socklen_t addrlen);
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(change_int(_port));
            addr.sin_addr.s_addr = inet_addr(_ip.c_str());
            socklen_t len = sizeof(sockaddr_in);
            int ret = bind(_fd,(struct sockaddr*)&addr,len);
            if(ret < 0){
                cerr<<"Socket bind error"<<endl;
                return false;
            } 
            return true;
        }
        bool Listen(){
            //int listen(int s, int backlog);
            //backlog  同一时间最大并发连接数
            int ret = listen(_fd,Max_backlog);
            if(ret < 0){
                cerr<<"Socket Listen error! "<<endl;
                return false;
            }
            return true;
        }
        //向服务器发送连接请求
        bool Connect(const string &srv_ip, 
                const string  &srv_port) {
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(change_int(srv_port));
            addr.sin_addr.s_addr = inet_addr(srv_ip.c_str());
            socklen_t len = sizeof(struct sockaddr_in);
            int ret = connect(_fd, (struct sockaddr*)&addr, len);
            if (ret < 0) {
                cerr << "connect error\n";
                return false;
            }
            return true;
        }
        //监听到有数据发送,就可以进行进行接收,并创建另外的套接字了,最早的套接字就是门迎套接字
        //
        //
        void Getnewfd(int newfd){
            _fd = newfd;
        }
        int GetFd() {
            return _fd;
        }
        bool Accept(TcpSocket &clisock){
            // newfd = int accept(int s, struct sockaddr *addr, socklen_t *addrlen);
            struct sockaddr_in addr;
            socklen_t len   = sizeof(struct sockaddr_in);
            int newfd = accept(_fd,(struct sockaddr*)&addr,&len);
            if( newfd < 0 ){
                cerr<<"Socket accept error "<<endl;
                return false ;
            }
            clisock.Getnewfd(newfd);     
            return true;
        }
        bool Send(string& data){
            //int send(int s, const void *msg, size_t len, int flags);
            //因为在TCP可以send说明建立的是点对点的连接,那么就不需要目的IP,以及端口号等等数据了
            int ret = send(_fd,&data[0],data.size(),0); 
            if(ret < 0){
                cerr<<"Data Send error"<<endl;
                return false ;
            }
            return true;
        }

        //接收来到的数据
        bool Recv(string& buf){
            //ssize_t recv(int sockfd, void *buf, size_t len, int flags);
            //flag = 0 ; 默认的是阻塞等待
            char temp[4096] = {0};
            ssize_t ret =recv(_fd,temp,4096,0);
            if(ret < 0){
                cerr<<"Recv error "<<endl;
                return false;
            }
            buf.assign(temp,ret);
            return  true;
        }
    private: 
           int _fd;

};


