#include"TCPsocket.hpp"



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
        CHECK(sock.Accept(clisock);
        string buf;
        sock.Recv(buf);
        cout<<"serv recv data :"<<buf<<endl;
        buf.clear();
        cout<<"serv send data: ";
        cin >> buf;
        cout<<buf;
        sock.Send(buf);
    } 
    sock.Close();
    return 0;
}
