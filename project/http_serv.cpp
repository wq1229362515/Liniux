#include"cpp-httplib/httplib.h"
#include<iostream>
#include<boost/filesystem.hpp>
#include<fstream>
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
            std::stringstream body;
            //body <<"<html><body>";
            for(;item_begin != item_end; ++item_begin) {
                if(bf::is_directory(item_begin->status())) {
                    continue;
                }              //判断文件信息是否是一个目录
                bf::path path = item_begin->path();
                std::string name = path.filename().string();
                //std::string path = item_begin->path().string();
                //std::cerr<<path<<std::endl;
                //rsp.body += name + "\n";
                //
                //body <<"<h4><a href='/list/"<<name <<"'>";
                //body <<name;
                //body <<"</a></h4>";
                rsp.body += name + "\n";
            }
            //body<<"</body></html>"; 
            ////rsp.body = body.str();
            rsp.status = 200;
            rsp.set_header("Content-Type","text/html");
        }

        //获取文件数据
        static void GetFileData(const Request &req, Response &rsp) {
            // a.txt ->Download/a.txt
            bf::path path (req.path);
            std::stringstream name;
            name << SHARE_PATH <<"/"<< path.filename().string();
            if(!bf::exists(name.str())) {
                rsp.status = 404;
                return;
            }
            if(bf::is_directory(name.str())) {
                rsp.status = 403;
                return;
            }
            std:: ifstream file(name.str(),std::ios::binary); 
            if(!file.is_open()) {
                std::cerr<<"open file"<<name.str()<<"failed\n";
                rsp.status = 404;
                return;
            }
            int64_t fsize = bf::file_size(name.str());
            rsp.body.resize(fsize);
            file.read(&rsp.body[0],fsize);
            if(!file.good()) {
                std::cerr<<"read file"<<name.str()<<"body error\n";
                rsp.status = 500;
                return ;
            }
            file.close();
            rsp.status = 200;
            rsp.set_header("Content-Type","application/octet-stream");
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



