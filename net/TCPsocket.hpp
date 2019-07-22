#pragma once

#include<iostream>
#include<sys/socket.h>
#include<sstream>
#include<string>
#include<stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
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
 * */

#define CHECK(q){if ((q) == false) return -1;}
#define Max_backlog 5
class TcpSocket {
    public:
        TcpSocket():_fd(-1){
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
        bool Bind(const string _ip,uint16_t _port){
            //int bind(int sockfd, struct sockaddr *my_addr, socklen_t addrlen);
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(_port);
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
        //将服务器发送连接请求
        bool Connect(const string &srv_ip, 
                const uint16_t &srv_port) {
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(srv_port);
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
        bool Accept(const string _ip,uint16_t _port){
            // newfd = int accept(int s, struct sockaddr *addr, socklen_t *addrlen);
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(_port);
            addr.sin_addr.s_addr = inet_addr(_ip.c_str());
            socklen_t len   = sizeof(struct sockaddr_in);
            int newfd = accept(_fd,(struct sockaddr*)&addr,&len);
            if( newfd < 0 ){
                cerr<<"Socket accept error "<<endl;
                return false ;
            }
            _fd = newfd;     
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
            ssize_t ret = (_fd,temp,4096,0);
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


