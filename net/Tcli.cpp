#include"TCPsocket.hpp"



int main (int argc ,char *argv[]){


    string ip = argv[1];
    stringstream temp;
    uint16_t port;
    temp << argv[2];
    temp >> port; 

    TcpSocket sock;
    CHECK(sock.Socket());

    while(1){
        sock.Connect(ip,port);
        string data;
        cout<<"client send data :";
        cin >> data;
        sock.Send(data);
        data.clear();
        sock.Recv(data);
        cout<<"client recv data :"<<data;

    }
    sock.Close();


    return 0;
}
