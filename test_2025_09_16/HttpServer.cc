#include "HttpServer.hpp"
#include <memory>

using namespace std;
using namespace server;

// 手册
static void Usage(string proc)
{
    cout << "\nUsage:\n\t" << proc << " local_port\n\t" << endl;
}

// 1. 服务器和网页分离
// 2. url为/, 这个/就是web根目录
// 回调方法(根据请求处理响应)
bool Get(const HttpRequest &req, HttpResponse &resp)
{
    cout << "----------------------------http start----------------------------" << endl;
    cout << req.inbuffer;
    cout << "method: " << req.method << endl;
    cout << "url: " << req.url << endl;
    cout << "httpversion: " << req.httpversion << endl;
    cout << "path: " << req.path << endl;
    cout << "----------------------------http end----------------------------" << endl;
    
    // 构建响应
    string respline = "HTTP/1.1 200 OK\r\n";
    string respheader = "Content-Type: text/html\r\n";
    string respblank = "\r\n"; // 报头
    string body = "<html><head><meta charset=\"UTF-8\"><title>Hello World</title></head><body>Hello world</body></html>";

    resp.outbuffer += respline;
    resp.outbuffer += respheader;
    resp.outbuffer += respblank;
    resp.outbuffer += body;

    return true;
}

// ./httpServer 8080
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(0);
    }

    uint16_t port = atoi(argv[1]);
    unique_ptr<HttpServer> httpsvr(new HttpServer(Get, port));
    httpsvr->initServer();
    httpsvr->startServer();

    return 0;
}