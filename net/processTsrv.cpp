#include"TCPsocket.hpp"
#include<signal.h>
#include<wait.h>
/*
 *  服务器端创建的流程
 *
 *  1.创建门迎套接字
 *  2.绑定地址端口等信息
 *  3.开始监听连接的到来
 *  开始监听就是告诉操作系统新的syn连接请求到来,就是服务器已经接收到了
 *  则为这个客户端新建立一个套接字专门用于与这个客户端进行独立的通信
 *  backlog 就是客户端的最大连接并发连接数
 *  4.为客户端建立新的socket accept
 *  使得这个套接字专门与指定的客户端进行数据通信
 *
 *
 *
 *
 *
 * */
void sigcb(int  no){
    // -1 等待任意的子进程退出
    //循环等待,不关心返回值
    //WNOHANG ,没有等到就不等了
    // > 0   有子进程退出
    // == 0  没有子进程退出
    //  < 0  出错
    // 当他>0了 就说明有子进程退出了然后处理完这个之后,就处理下一个,
    // 循环对僵尸进程进行处理,没有子进程退出就退出循环,
    // 处理完所有的僵尸进程 
    while(waitpid(-1,NULL,WNOHANG) > 0);
}
int main (int argc ,char *argv[]){

    
    if(argc != 3){
        cout<<"./Tsrv ip port"<<endl;
        return -1;
    }
    //SIGCHLD 这就是子进程退出给父进程发送的信号
    //非可靠信号
    signal(SIGCHLD,sigcb);
    string ip = argv[1];
    stringstream temp;
    uint16_t port;
    temp << argv[2];
    temp >> port;

    TcpSocket sock;
    CHECK(sock.Socket());
    CHECK(sock.Bind(ip,port));
    CHECK(sock.Listen());
    /*
     * 因为服务端不知道客户端的新连接请求什么时候到来
     * 因此在程序中的体现就是会卡在accept或者recv两个接口处
     * 导致流程无法进行
     * 解决的方案就是服务端每一个新的客户端都创建一个进程/线程来与客户端
     * 进行通信
     * 两种方法:线程占用资源少但是不稳定
     *
     * 进程占用资源多但是稳定
     * 权衡的考虑这两种方法,各有各的好处
     *
     *
     * */
    while(1){
        //连接套接字
        TcpSocket clisock;
        CHECK(sock.Accept(clisock));
        if(fork() == 0){
            while(1){
                string buf;
                clisock.Recv(buf);
                cout<<"serv recv data :"<<buf<<endl;
                buf.clear();
                cout<<"serv send data :";
                fflush(stdout);
                cin >> buf;
                clisock.Send(buf);
            }
        }
        //这个时候父进程也有一份和子进程一样的套接字描述符,但是父进程并不是来处理这个套接字的
        //父进程要干的事情就是接受这个连接请求,然后把这个通信交给子进程来干,所以要关闭这个父进程中的
        //文件描述符
        clisock.Close();
        //子进程处理,那么子进程退出之后就会成为僵尸进程,wait 等待子进程退出,但是如果子进程不退出
        //那么就会卡在wait这里,又构成阻塞了
        //信号来解决,子进程退出会给父进程发送一个信号,什么时候信号到来了,就处理他
    } 
    sock.Close();
    return 0;
}
