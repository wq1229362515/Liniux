#pragma once 

#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<iostream>
#include<string>
#include<unistd.h>
#include <errno.h>

using namespace std;
//面向无连接,不可靠,面向字节流服务 
//创建UDP套接字类
//1.创建 套接字文件描述符
//内核中建立结构体(struvt socket),进程和网卡之间建立联系
/*
int socket(int domain, int type, int protocol);

int domain //   (IP地址类型)

AF_INET             IPv4 Internet protocols          
AF_INET6            IPv6 Internet protocols 

int type(TCP/UDP的传输数据的方式)
SOCK_STREAM  面向字节流 (TCP)
SOCK_DGRAM   面向数据报  Supports datagrams

int protocol  (采取什么协议来传输,默认是0,系统会自动推演)
 IPPROTO_TCP 和 IPPTOTO_UDP


*/
//2.绑定端口号，服务器需要绑定但是客户端不一定需要绑定
/* 
 *
 * int bind(int sockfd,const struct sockaddr *my_addr, socklen_t addrlen); 成功返回0,否则返回-1,并设置错误代码
 * bind 函数的作用是绑定IP地址,当用户进程中需要来绑定这个IP地址和端口号了,那么就需要调用,当不需要绑定的时候,
 * 比如说我们的客户端,那么就不需要用bind来绑定
 * 因为一旦绑定了,那么就没办法再开一个进程了,IP+Port 标识了网络上某一台主机的某一个进程
 * 一个进程可以绑定多个端口号,但是一个端口号不能被多个进程绑定
 * 示情况而定,看需不需要绑定,
 * 
 *  sockfd 套接字文件描述符
 *
 *  struct sockaddr *my_addr   地址信息,sa_family(存放的是地址域) 当前地址是哪一个地址信息 
 *                              第二个是补全位
 *   
 *  使用同一的addr,不同的addr,
 *  sockaddr 为了保持程序的通用性,将IP4/IP6 都强转成 struct  sockaddr 类型的
 *  Internet的地址定义在<netinet/in.h>，在IPV4因特网中(AF_INET)中，套接字的地址结构如下：
    包含了三个内容 地址类型源端口号,源地址,
    IPv4的地址信息结构2个字节的family
    sockaddr_in6
   struct sockaddr_in{
        sa_family_t sin_family;     //设置的是地址类型
        in_port_t sin_port;
        struct in_addr sin_addr;
        }
   struct in_addr{
        in_addr_t s_addr;
    }
 *  socklen_t addrlen 这个为addr 的大小可以由 sizeof 计算得到
 *
 *
 *
 * */
//3.ssize_t send(client)
//ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
//                      const struct sockaddr *dest_addr, socklen_t addrlen);
//                      //目的端口地址
//  ssize_t send(int sockfd, const void *buf, size_t len, int flags);
//
//4.ssize_t recefrom(client)
//ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
//                        struct sockaddr *src_addr, socklen_t *addrlen);
//                       //源IP地址 
//5.关闭套接字文件 close(fd);
/*
 *
 *
 * netstat 是查看网络状态的重要的工具
 * n拒绝显示别名
 * l仅列出仅在监听的服务状态
 * t(tcp) 相关选项
 * u(udp) 仅仅显示udp相关选项
 * a(all)显示所有的选项
 * p 显示建立相关链接的程序名
 *
 *
 *
 *
 *
 *
 * */
#define CHECK(q) {if((q) == false ) return -1;}
class UdpSocket{

    public:
        //构造函数
        UdpSocket ():
            fd_(-1){
        }
        bool Socket(){
            fd_ = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
            if(fd_ < 0){
                perror("creat socket fail!\n");
                return false;
            }
            return true;
        }        
        void Close(){
            close(fd_);
        }
        bool Bind(const string& ip,const uint16_t port){
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            //将端口在传输的时候,将主机字节序转换为网络字节序
            addr.sin_port = htons(port);
            //cout<<port<<endl;
            //将ip地址也就是string类型的转换为IP地址
            addr.sin_addr.s_addr = inet_addr(ip.c_str());
            socklen_t len = sizeof(struct sockaddr_in);
            int ret = bind(fd_,(struct sockaddr*)&addr,len);
                if(ret < 0){
                    perror("bind error");
                    return false;
                }
                return true;
        }
        bool Send(const string &data,const string& ip,const uint16_t port){
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            //将点分十进制IP地址转换为IP地址
            addr.sin_addr.s_addr = inet_addr(ip.c_str());
            addr.sin_port = htons(port); 
            socklen_t len = sizeof(struct sockaddr_in);
            ssize_t ret = sendto(fd_,&data[0],data.size(),0,(struct sockaddr*)&addr,len);
            if(ret < 0){
                perror("send error");
                return false;
            }
            cout<<"send byte: "<<data.size()<<endl;
            return true;
        }
        //ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
        //                        struct sockaddr *src_addr, socklen_t *addrlen);
        //                       //源IP地址 
        //接受套接字文件放入到buf里面
        
        bool Ricv(string&  buf,string & ip,uint16_t&  port){
            char temp[4096] = {0}; 

            struct sockaddr_in addr; 
            socklen_t len = sizeof(struct sockaddr_in);
            cout<<"Test"<<endl;

            int ret = recvfrom(fd_,temp,4096,0,(struct sockaddr*)&addr,&len);
            cout<<"Test"<<endl;
            if(ret < 0){
                perror("recvfrom error");
                return false;
            }
            //将接受到的缓存数据放入缓存中去
            //将IP地址转换为string点分十进制
            ip = inet_ntoa(addr.sin_addr);
            //网络字节序转换为主机字节序
            //htons(port)
            //n代表network
            //h代表host
            //s代表16位短整形
            port = ntohs(addr.sin_port);
            buf.assign(temp,ret);
            cout<<"recv byte:"<<buf.size()<<endl;
            return true;
        }
        




        
        
    private: int fd_;
};
