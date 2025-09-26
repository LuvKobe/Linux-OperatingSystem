#include "HttpServer.hpp"
#include "Util.hpp"
#include <memory>

using namespace server;

// 手册
static void Usage(string proc)
{
    cout << "\nUsage:\n\t" << proc << " local_port\n\t" << endl;
}

// 请求资源类型
string suffixToDesc(const string suffix)
{
    string ct = "Content-type: ";
    if (suffix == ".html")
        ct += "text/html";
    else if (suffix == ".jpg")
        ct += "application/x-jpg";
    
    ct += "\r\n";
    return ct;
}

// 1. 服务器和网页分离
// 2. url为/, 这个/就是web根目录
// 3. 我们要正确的给客户端返回资源类型, 我们首先要自己知道！(所有的资源都有后缀)
 
// 回调方法(根据请求处理响应)
bool Get(const HttpRequest &req, HttpResponse &resp)
{
    // 提交的表单对应的服务: /test.py ?account=123456&passwd=000000
    if (req.path == "test.py")
    {
        // 建立进程间通信 pipe
        // fork创建子进程, execl("/bin/python", test.py);
        // 父进程, 将 req.parm 通过管道写给某些后端语言, py/java/php等
    }

    // 
    cout << "----------------------------http start----------------------------" << endl;
    cout << req.inbuffer;
    cout << "method: " << req.method << endl;
    cout << "url: " << req.url << endl;
    cout << "httpversion: " << req.httpversion << endl;
    cout << "path: " << req.path << endl;
    cout << "suffix: " << req.suffix << endl;
    cout << "size: " << req.size << "字节" << endl;
    cout << "----------------------------http end----------------------------" << endl;
    
    // 构建响应
    string respline = "HTTP/1.1 200 OK\r\n";

    //string respheader = "Content-Type: text/html\r\n";
    string respheader = suffixToDesc(req.suffix); // 后缀
    if (req.size > 0) // 正文
    {
        respheader += "Content-Length";
        respheader += to_string(req.size);
        respheader += "\r\n";
    }
    //respheader += "Set-Cookie: 123456abcdefg\r\n"; // 把我们的信息写入到浏览器Cookie
    respheader += "Set-Cookie: name=123456abcdefg; Max-Age=120\r\n"; // 把我们的信息写入到浏览器Cookie, 设置120秒失效
    // 往后, 每次http请求, 都会自动携带曾经设置的所有的cookie, 帮服务器进行鉴权行为 --> 进而支持http会话保持

    string respblank = "\r\n"; // 报头

    //string body = "<html><head><meta charset=\"UTF-8\"><title>Hello World</title></head><body>Hello world</body></html>";
    string body; // 从文件里面读取
    // 把文件路径中的内容读取到body中
    if (!Util::readFile(req.path, &body)) 
    {   
        // 读取失败, 返回404页面
        Util::readFile(html_404, &body); 
    } 

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

    //根据你输入的对应的路径, 给你绑定方法。这就是所谓的http的【功能路由】
    // httpsvr->registerCb("/", Get);
    // httpsvr->registerCb("/search", Search);
    // httpsvr->registerCb("test.py", Other);
    
    httpsvr->initServer();
    httpsvr->startServer();

    return 0;
}