/*=============================================================== 
 *   Copyright (C) . All rights reserved.")
 *   文件名称： 
 *   创 建 者：zhang
 *   创建日期：
 *   描    述：tcp服务端通信流程
 *       1. 创建套接字
 *       2. 为套接字绑定地址信息
 *       3. 开始监听
 *       4. 获取已完成连接socket
 *       5. 通过获取的新建socket与客户端进行通信-接收数据
 *       6. 发送数据
 *       7. 关闭套接字 
 ================================================================*/
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include "tcpsocket.hpp"

void sigcb(int no) {
    //如果有僵尸进程可以处理，就一直处理
    //如果没有子进程退出了则waitpid返回0，退出循环
    while(waitpid(-1, NULL, WNOHANG) > 0);
}
int main(int argc, char *argv[])
{
    if (argc != 3) {
        std::cerr << "./tcp_srv ip port\n";
        return -1;
    }
    signal(SIGCHLD, sigcb);
    TcpSocket lst_sock;
    /*1. 创建套接字*/
    CHECK_RET(lst_sock.Socket());
    /*2. 为套接字绑定地址信息*/
    CHECK_RET(lst_sock.Bind(argv[1], argv[2]));
    /*3. 开始监听*/
    CHECK_RET(lst_sock.Listen());
    while(1){
        /*4. 获取已完成连接socket*/
        TcpSocket clisock;
        bool ret = lst_sock.Accept(clisock);
        if (ret == false) {
            continue;
        }
        if (fork() == 0) {
            while(1) {
                /*5. 通过获取的新建socket与客户端进行通信-接收数据*/
                std::string buf;
                clisock.Recv(buf);
                std::cout << "client say: " << buf << std::endl;
                /*6. 发送数据*/
                buf.clear();
                std::cout << "server say: ";
                fflush(stdout);
                std::cin >> buf;
                clisock.Send(buf);
            }
            clisock.Close();
        }
        clisock.Close();
    }
    /*7. 关闭套接字 */
    lst_sock.Close();
    return 0;
}

