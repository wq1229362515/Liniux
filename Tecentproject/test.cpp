#include"cpp-httplib/httplib.h"
#include<iostream>
#include<unistd.h>

using namespace httplib;
using namespace std;


static int i = 0;
void Hello (const Request &req, Response &rsp) {


    rsp.status = 302;
    rsp.set_header("Location","http://www.baidu.com");
    ++i;
    cout<<"连接次数: "<<i<<endl;
    return;
}
void Hi(const Request &req, Response &rsp) {

    rsp.status = 200;
    rsp.set_content("hello","text/html");
    return ;
}

int main () {
    Server server;
    server.Get("/",Hello);
    server.Get("/hello",Hi);

    //设置静态资源目录
    server.set_base_dir("resource");




    server.listen("0.0.0.0",9000);
    //while(1);
    return 0;
}
