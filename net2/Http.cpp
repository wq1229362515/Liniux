

/*
 *
 *
 * Http ---超文本传输协议(HyperText Transfer Protocol)
 *
 * 超文本标记语言(Hyper Text Markup Language)
 *
 * “超文本”就是指页面内可以包含图片、链接，甚至音乐、程序等非文字元素。
 * 超文本标记语言(或超文本标签语言)的结构包括“头”部分、和“主体”部分，
 * 其中“头”部提供关于网页的信息，“主体”部分提供网页的具体内容。
 * 万维网（world wide web）上的一个超媒体文档称之为一个页面（外语：page）。
 * 作为一个组织或者个人在万维网上放置开始点的页面称为主页
 * 主页中通常包括有指向其他相关页面或其他节点的指针（超级链接）
 * 所谓超级链接，就是一种统一资源定位器（Uniform Resource Locator，外语缩写：URL）指针
 * 通过激活（点击）它，可使浏览器方便地获取新的网页。
 * 这也是HTML获得广泛应用的最重要的原因之一。在逻辑上将视为一个整体的一系列页面的有机集合称为网站
 * 超级文本标记语言（英文缩写：HTML）是为“网页创建和其它可在网页浏览器中看到的信息”设计的一种标记语言
 * 
 *
 * 了解了超文本是什么,那么HTTP就是对客户端和服务器端之间传输超文本的时候的传输动作所作出的规定
 *
 *
 * 整个基本的工作流程是客户端发送一个HTTP请求，说明客户端想要访问的资源和请求的动作
 * 服务端收到请求之后，服务端开始处理请求，并根据请求做出相应的动作访问服务器资源，最后通过发送HTTP响应把结果返回给客户端。
 * 其中一个请求的开始到一个响应的结束称为事务，当一个事物结束后还会在服务端添加一条日志条目
 *
 * 
 *
 * 统一资源定位器（Uniform Resource Locator，外语缩写：URL）指针
 *
 * HTTP 请求报文
 *
 * 状态行 + 请求头 +请求报文
 *
 * 状态行 : 包括请求方式Method,资源路径 URL,协议版本 Version;
 *
 * Method---- GET POST PUT DELETE(获取,修改,上传,删除)
 * HTTP/1.0支持：GET、POST、HEAD三种HTTP请求方法。
 *
 *
 * POST把传递的数据封装在HTTP请求数据中，以名称/值的形式出现，可以传输大量数据，对数据量没有限制，也不会显示在URL中。
 * 表单的提交用的是POST。
 *
 * GET 没有请求正文,提交数据在URL
 * 传递参数长度受限制，因为传递的参数是直接表示在地址栏中，而特定浏览器和服务器对url的长度是有限制的。
 * 因此，GET不适合用来传递私密数据，也不适合拿来传递大量数据。
 *
 * [方法]+[URL]+[版本号]
 *
 *
 * 请求头 : 包括一些访问的域名,用户代理,Cookie 等信息
 * 请求的属性,冒号分割键值对,每组属性之间使用\n分割,遇到空行就是结束
 *
 *
 * 请求正文: 就是HTTP请求的数据
 *
 * POST /user HTTP/1.1      //请求行
 * Host: www.user.com
 * Content-Type: application/x-www-form-urlencoded
 * Connection: Keep-Alive
 * User-agent: Mozilla/5.0.      //以上是首部行
 * （此处必须有一空行）  //空行分割header和请求内容 
 * name=world   请求体
 *
 *
 * HTTP 响应报文
 *
 * 状态行+ 响应头+ 响应正文
 *
 * 服务器收到了客户端发来的HTTP请求后，根据HTTP请求中的动作要求，
 * 服务端做出具体的动作，将结果回应给客户端，称为HTTP响应。
 * 
 * 状态行:服务协议/version版本 状态码 回应短语
 *
 * 响应头:服务器,发送响应时间等等 
 *
 *
 * 响应正文：就是响应的具体数据
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 * */

#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string>
#include<string.h>
#include <stdio.h>


#define MAX_BLOG 5
int main (int argc ,char* argv[]){


    if(argc != 3){
        cerr<<"./HTTP  192.168.247.128 9000"<<endl;
        return -1;
    }
    uint16_t port = 0;
    stringstream temp;
    temp << argv[2];
    temp >> port;

    //1.服务器创建socket套接字
    //2.bind绑定地址信息
    //3.listen() 监听连接到来
    //在循环中去做这些事情
    //
    //4.accept() 为连接建立新的套接字
    //5.读取数据read()
    
    //int socket(int domain, int type, int protocol);
    int sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(sockfd < 0){
        cerr<<"socket http error"<<endl;
        return -1;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port); 
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    //int bind(int sockfd, struct sockaddr *my_addr, socklen_t addrlen);
    int ret = bind(sockfd,(sockaddr*)&addr,sizeof(addr));
    if( ret < 0 ){
        cerr<<"bind error"<<endl;
        return -1;
    }

    //int listen(int s, int backlog);
    ret = listen(sockfd,MAX_BLOG);
    if( ret < 0 ){
        cerr<<"listen error"<<endl;
        return -1;
    }

    while(1){
        
        // int accept(int s, struct sockaddr *addr, socklen_t *addrlen);
        socklen_t len = sizeof(addr);
        int newsock = accept(sockfd,(sockaddr*)&addr,&len);
        if(newsock <  0){
            cerr<<"newsock error"<<endl;
            continue;
        }
        char input_data[1024 *10] = {0};

        //ssize_t read(int fd, void *buf, size_t count);
        ssize_t read_size = read(newsock,input_data,sizeof(input_data)-1);
        if(read_size  <  0){
            cerr<<"read error"<<endl;
            return -1;
        }
        cout<<"[Resquest]"<<input_data<<endl;

        char buf[1024] = {0};

        const char *hello = "<h1> 有你 你是美好滴....\n 有你 你是美好滴....\n</h1>";


        sprintf(buf,"HTTP/1.0 200 OK\nnContent-Length:%lu\n\n%s",strlen(hello),hello);
        write(newsock,buf,strlen(buf));
        

    }


     return 0;
 }





