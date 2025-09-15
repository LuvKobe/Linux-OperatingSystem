#include "calServer.hpp"
#include <memory>

using namespace server;
using namespace std;

// 手册
static void Usage(string proc)
{
    cout << "\nUsage:\n\t" << proc << " local_port\n\t" << endl;
}

// req 里面一定是我们处理好的一个完整的对象
// resp: 根据req进行业务处理, 填充resp, 不用管理任何IO(读取和请求)、序列化、反序列化等任何细节
bool cal(const Request &req, Response &resp)
{
    // req已经有结构化完成的数据了, 可以直接用
    resp.exitcode = OK;
    resp.result = OK;

    switch (req.op)
    {
    case '+':
        resp.result = req.x + req.y;
        break;
    case '-':
        resp.result = req.x - req.y;
        break;
    case '*':
        resp.result = req.x * req.y;
        break;
    case '/':
    {
        if (req.y == 0)
            resp.exitcode = DIV_ZERO; // 除0错误
        else
            resp.result = req.x / req.y;
    }
    break;
    case '%':
    {
        if (req.y == 0)
            resp.exitcode = MOD_ZERO; // 除0错误
        else
            resp.result = req.x % req.y;
    }
    break;
    default:
        resp.exitcode = OP_ERROR;
        break;
    }

    return true;
}

// tcp服务器启动上, 和udp服务器一模一样
// ./tcpserver local_port
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }
    uint16_t port = atoi(argv[1]);

    unique_ptr<CalServer> tsvr(new CalServer(port));
    tsvr->initServer();
    tsvr->startServer(cal);
    
    return 0;
}
