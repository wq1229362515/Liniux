#include"cpp-httplib/httplib.h"

using namespace httplib;
 
//typedef std::function<void(const Request &, Response &)> Handler;
void Helloworld(const Request &req, Response &rsp) {

    rsp.status = 302;
    //void set_content(const std::string &s, const char *content_type);
    //rsp.set_content();
    //void set_header(const char *key, const std::string &val);
    rsp.set_header("Location","http://www.baidu.com");
    rsp.set_content("<hi> hello </hi>","text/html");

    
    return ;
}
void hi(const Request &req, Response &rsp) {

    rsp.status = 200;
    rsp.set_content("<hi> hello </hi>","text/html");

    return ;
}
int main () {

    Server server;
    server.Get("/",Helloworld); 
    server.Get("/hi",hi);
    server.listen("0.0.0.0",9000);

    return 0;
}
