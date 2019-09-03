#include"cpp-httplib/httplib.h"
#include<string>
#include<iostream>
#include<fstream>
#include<vector>
#include<ifaddrs.h>
#include<netinet/in.h>
#include<boost/filesystem.hpp>
#include<arpa/inet.h>
#include<boost/algorithm/string.hpp>
#include<thread>


#define RANGE_SIZE (1024*1024*10)
using namespace httplib;
using namespace std;
namespace bf = boost::filesystem;
class Download {

    private:
        string _host;
        string _name;
        int64_t _start;
        int64_t _end;
        int64_t _len;
    public:
        bool _res;
    public:
        Download():_res(false) {
        }
        bool Start() {
            Client client(_host.c_str(),);
        }
        void SetData(string &host,string &name,int64_t &start,int64_t &end,int64_t &len) {

            _host = host;
            _name = name;
            _start = start;
            _end = end;
            _len = len;
        }
        
};


class P2PClient {

    private:
        uint16_t _srv_port;
        int _host_inx;                               //用户选中的IP地址
        std::vector<std::string> _online_list;      //在线主机列表
        std::vector<std::string> _file_list;        //文件列表

    public:
        P2PClient(uint16_t port):_srv_port(port){
        }
        bool GetAllHost(std::vector<std::string>&list) {
            // int getifaddrs(struct ifaddrs **ifap);
            struct ifaddrs *addrs = NULL;
            struct sockaddr_in *ip = NULL;
            struct sockaddr_in *mask = NULL;
            getifaddrs(&addrs);
            for(;addrs != NULL;addrs = addrs->ifa_next) {
                //获取
                ip = (struct sockaddr_in*) addrs->ifa_addr;
                mask = (struct sockaddr_in*) addrs->ifa_netmask;
                if(ip->sin_family != AF_INET) {
                    continue;
                }
                if(ip->sin_addr.s_addr == inet_addr("127.0.0.1")) {
                    continue;
                }
                uint32_t net,host;
                net = ntohl(ip->sin_addr.s_addr & mask->sin_addr.s_addr);
                host = ntohl(~mask->sin_addr.s_addr);
                for(int i = 2;i< host-1 ;i++) {
                    struct in_addr ip;
                    ip.s_addr = htonl(net+i);
                    list.push_back(inet_ntoa(ip));
                }
            }
            freeifaddrs(addrs);
            return true;
        }
        void HostPair(std::string &i) {
            Client client (&i[0],_srv_port);
            auto rsp = client.Get("/hostpair");
            if(rsp && rsp->status == 200) {
                cerr<<"host"<<i<<"pair success"<<endl;
                _online_list.push_back(i);
            }
            //std::cerr<<"host"<<i<<"pair failed"<<endl;
            cout<<i.c_str()<<endl;
            return;
        }
        bool GetOnlineHost (std::vector<std::string>&list) {
            _online_list.clear();           //清空内容,重新添加
            std::vector<std::thread> thr_list(list.size());
            for(int i = 0;i< list.size();i++) {
                std::thread thr(&P2PClient::HostPair,this,std::ref(list[i]));
                thr_list[i] = std::move(thr);
            }
            for(int i = 0;i<thr_list.size();++i) {
                thr_list[i].join();
            }
            return true;
        }
        bool ShowOnlineHost() {
            for(int i = 0;i < _online_list.size();i++) {
                std::cout<< i<<  ". "<<_online_list[i]<<"\n";
            }
            std::cout<<"please choose Online Host :"; fflush(stdout);
            std::cin >> _host_inx;
            if(_host_inx <0 || _host_inx > _online_list.size()){
                _host_inx  = -1;
                std::cerr<<"choose error\n";
                return false;
            }
            return true;
        }
        bool GetFileList() {            //获取文件列表放在filelist里面)
            Client client(_online_list[_host_inx].c_str(),_srv_port);
            auto rsp = client.Get("/list");
            if(rsp && rsp->status == 200) {
                //解析正文切割
                boost::split(_file_list,rsp->body,boost::is_any_of("\n"));
            }
            return true;
        }
        bool ShowFileList(std::string &name) {
            for(int i = 0;i<_file_list.size();i++) {
                std::cout<<i<<" ." <<_file_list[i]<<"\n";
            }
            std::cout<<"please choose:";
            fflush(stdout);
            int file_idx;
            std::cin>>file_idx;
            if(file_idx <0|| file_idx> _file_list.size()) {
                std::cerr<<"choose error\n";
                return false;
            }

            name = _file_list[file_idx];
            return true;
        }
        void RangeDownload(Download *dl) {
            dl->Start();
            return ;
        }
        int64_t GetFileSize() {

        }
        bool DownloadFile(std::string &name) {
            //GET  /list/filename HTTP/1.1
            //HTTP/1.1 200 OK
            //Content-Length:fsize (文件的长度)
            //
            //FILE
            //向服务端发送请求 
            

            std::string host = _online_list[_host_inx];
            Get
            std::string uri = "/list/"+name;                     //统一资源标识符
            Client client(host.c_str(),_srv_port);
            auto rsp = client.Head(uri.c_str());
            if(rsp && rsp->status ==200) {
                if(rsp->has_header("Content-Length")) {
                    // 可以获取文件长度进行分块下载
                    int64_t content_len; 
                    std::string len = rsp->get_header_value("Content-Length");
                    std::stringstream tmp;
                    tmp <<len;
                    tmp >> content_len;
                    int count = content_len/RANGE_SIZE;


                    std::vector<std::thread> thr_list(count+1);
                    std::vector<Download> res_list(count+1);
                    for(int i = 0;i<= count;i++) {
                        //1000 300 0-299 300-599 600-899 900-999
                        int64_t start = i*RANGE_SIZE;
                        int64_t end = (i+1)*RANGE_SIZE -1;
                        if(i == count) {
                            if((content_len % RANGE_SIZE ) == 0) 
                                break;
                            end = content_len -1;
                        }                       
                        int64_t len = end -start +1;
                        res_list[i].SetData(host,name,start,end,len);
                        std::thread thr(&P2PClient::RangeDownload,this,
                                &res_list[i]);
                        thr_list[i] = std::move(thr);
                    }
                    bool ret = true;
                    for(int i = 0;i<=count;++i) {
                        thr_list[i].join();
                        if(res_list[i]._res == true) {
                            continue;
                        }
                        ret = false;
                    }
                    if(ret == false) {
                        std::cerr<<"download file" <<name<<" failed"<<endl;
                    }
                }
            else {
                //没有文件长度,无法进行分块下载
                //1.正常下载
                //2.报错,当前方案不支持
                    std::cerr<<"download file" <<name<<" failed"<<endl;
                    return false;
                }
            }
            else {
                    std::cerr<<"download file" <<name<<" failed"<<endl;
                    return false;
            }
            std::cout<<"download success"<<name<<" success"<<endl;
            return true;
        }

        int DoFace() {
            
            std::cout<<"1.搜索附近主机"<<endl;
            std::cout<<"2.显示在线主机"<<endl;
            std::cout<<"3.显示文件列表"<<endl;
            std::cout<<"0.退出"<<endl;
            std::cout<<"please choose:";
            fflush(stdout);
            int choose;
            std::cin>>choose;
            return choose;
        }
    public:
        bool start() {
            while(1) {
                int choose = DoFace();
                std::vector<std::string> list;
                switch(choose) {
                    case 1: 
                        GetAllHost(list);
                        GetOnlineHost(list);
                        break;
                    case 2:
                        if(!ShowOnlineHost()) {
                            break;
                        }
                        GetFileList();
                        break;
                    case 3: {
                                string filename;
                                if(ShowFileList(filename) == false) {
                                    break;
                                }
                                DownloadFile(filename);
                                break;
                            }
                    case 0:
                        exit(0);
                        break;
                    default:
                        break;
                }
            }
        }
};
int main(){
    P2PClient client(9000);
    client.start();
    return 0;
}
