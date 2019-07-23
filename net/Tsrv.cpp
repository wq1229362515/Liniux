#include"TCPsocket.hpp"

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
int main (int argc ,char *argv[]){

    
    if(argc != 3){
        cout<<"./Tsrv ip port"<<endl;
        return -1;
    }
    string ip = argv[1];
    stringstream temp;
    uint16_t port;
    temp << argv[2];
    temp >> port;

    TcpSocket sock;
    CHECK(sock.Socket());
    CHECK(sock.Bind(ip,port));
    CHECK(sock.Listen());
    while(1){
        //连接套接字
        TcpSocket clisock;
        //无法持续和每一个客户端进行通信,阻塞有两种情况
        //accept是一个阻塞的操作
        //和每一个客户端进行连接之后,就会接受新的连接,不会再和原来的客户端进行通信了(应该说的是卡在
        //accept那一块,也就是说只能去通信一次,就上去接收新连接了,所以你客户端再发过去他正在等待接受新连接不会去理会的)
        //
        //所以就需要线程或者进程
        //主进程或者线程的任务只是处理这个链接的请求,等到新连接来了,重新创建一个线程或者进程,把这个通信的任务
        //专门交给了线程或者是进程去处理,这样的话解决的就是accept接受连接阻塞的问题,就算是阻塞,也只是阻塞在了
        //接受数据那一块
        //所以就实现了多个执行流与单个客户端进行通信
        CHECK(sock.Accept(clisock));
        string buf;
        //Recv默认也是阻塞的
        //第二个客户端也上来了,但是正在等待接受第一个客户端的消息
        clisock.Recv(buf);
        cout<<"serv recv data :"<<buf<<endl;
        buf.clear();
        cout<<"serv send data : ";
        fflush(stdout);
        cin >> buf;
        cout<<buf;
        clisock.Send(buf);
    } 
    sock.Close();
    return 0;
}
