#include"cpp-httplib/httplib.h"
#include<iostream>
#include<boost/filesystem.hpp>
using namespace httplib;
namespace bf = boost::filesystem;
#define SHARE_PATH "Download"       //共享的目录
class P2PServer {

    private:
        Server _server;

        static void GetHostPair(const Request &req, Response &rsp) {
            rsp.status = 200;
        }
        static void GetFileList(const Request &req, Response &rsp) {
            bf::directory_iterator item_begin(SHARE_PATH);    //  
            bf::directory_iterator item_end;                //
            for(;item_begin != item_end; ++item_begin) {
                if(bf::is_directory(item_begin->status())) {
                    continue;
                }              //判断文件信息是否是一个目录
                std::string path = item_begin->path().string();
                std::cerr<<path<<std::endl;
                rsp.body += path + "\n";
            }
            rsp.status = 200;
            //rsp.set_content(&body[0],body.size(),"text/plain");
        }
        static void GetFileData(const Request &req, Response &rsp) {
        }
    public:
        P2PServer() {
            //boost 库方便移植
            // 判断共享目录弱不存在则创建
            if(!bf::exists(SHARE_PATH)) {
                bf::create_directory(SHARE_PATH);
            }
        }
        bool Start(uint16_t port) {
            _server.Get("/hostpair",GetHostPair);
            _server.Get("/list",GetFileList);
            _server.Get("/list/(.*)",GetFileData);
            _server.listen("0.0.0.0",port);
        }



};
int main () {


    P2PServer srv;
    srv.Start(9000);



    return 0;
}



