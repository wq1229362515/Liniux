#include"UDPsocket.hpp"
#include<sstream>


int main (int argc,char *argv[]){


    if(argc < 3){
        cout<<"./client ip port"<<endl;
        return 0 ;
    }
    //客户端是做出发送数据的动作
    
    uint16_t port;
    string ip = argv[1];
    stringstream tmp;
    tmp << argv[2];
    tmp >> port;

    UdpSocket cli;
    CHECK(cli.Socket());


    while(1){
    
        string buf;

        cout<<"client say:  ";
        cin >>buf;
        cli.Send(buf,ip,port);
        cout<<"Send success !"<<endl;
        buf.clear();
        cli.Ricv(buf,ip,port);
        cout<<"serv say:  "<<buf<<endl;
        buf.clear();
        

    }
    cli.Close();

    return 0;
}
