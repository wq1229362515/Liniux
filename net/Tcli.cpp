#include"TCPsocket.hpp"
#include<signal.h>
/*
 *
 *
 *  客户端建立连接的过程
 *
 *  1.创建套接字程序
 *  2.为套接字绑定地址信息(可绑定也可以不绑定)
 *  3.向服务器端口发送建立连接请求  connect
 *  4.发送数据
 *  5.接受数据
 *  5.关闭套接字 
 *
 *
 * */
void sigcb(int sigo){
    cout<<"recv a signal SIGPIPE------connect shutdown"<<endl;
}
int main (int argc ,char *argv[]){


    string ip = argv[1];
    stringstream temp;
    uint16_t port;
    temp << argv[2];
    temp >> port; 
    signal(SIGPIPE, sigcb);
    TcpSocket sock;
    CHECK(sock.Socket());
    //客户端只有在建立连接之后才可以循环的进行发送
    //数据或者接受数据的操作,不可以写在循环里面否则的话,
    //在接受数据之后就开始重新建立连接
    sock.Connect(ip,port);

    while(1){

        string data;
        cout<<"client send data :";
        fflush(stdout);
        cin >> data;
        sock.Send(data);
        data.clear();
        sock.Recv(data);
        cout<<"client recv data :"<<data<<endl;

    }
    sock.Close();
    return 0;
}
