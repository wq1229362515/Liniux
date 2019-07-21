#include"UDPsocket.hpp"
#include<sstream>



int main (int argc, char* argv[]){

    if(argc != 3){
        cout<<"./udp_srv IP port"<<endl;
        return 0;
    }
    uint16_t port;
    string ip = argv[1];
    stringstream tmp;
    tmp << argv[2];
    tmp >>port;

    UdpSocket serv;
    CHECK(serv.Socket());
    CHECK(serv.Bind(ip,port));


    //服务器应答客户端发出的信号
    while(1){
        string buf;
        string ip;
        uint16_t port;
        //服务器首先要接受发过来的信号
        serv.Ricv(buf,ip,port);
        cout<<"client say :"<<buf<<endl;
        cout<<"servi say  :";
        buf.clear();
        cin>>buf;
        //做出应答
        serv.Send(buf,ip,port);

    }
    serv.Close();
}
