#include"TCPsocket.hpp"
#include<signal.h>
#include<stdlib.h>





void sigcb(int sigo){
    cout<<"recv a signal SIGPIPE------connect shutdown"<<endl;
}
int main (int argc ,char *argv[]){


    string _ip = argv[1];
    string _port = argv[2];
    signal(SIGPIPE, sigcb);
    TcpSocket sock;
    CHECK(sock.Socket());
    //客户端只有在建立连接之后才可以循环的进行发送
    //数据或者接受数据的操作,不可以写在循环里面否则的话,
    //在接受数据之后就开始重新建立连接
    sock.Connect(_ip,_port);

    while(1){

        Calu data(10,20,'+');
        cout<<"client send data :"<<data._num1<<data._Op<<data._num2;
        fflush(stdout);
        //int send(int s, const void *msg, size_t len, int flags);
        //序列化
        int fd = sock.GetFd();
        int ret = send(fd,(char *)&data,sizeof(data),0);
        exit(0);
        if(ret < 0){
            cerr<<"cli send data error"<<endl;
        }
    }
    sock.Close();
    return 0;
}
