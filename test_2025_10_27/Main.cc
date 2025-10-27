#include "TcpServer.hpp"
#include <memory>

using namespace tcpserver;

// 手册
static void Usage(string proc)
{
    cout << "\nUsage:\n\t" << proc << " port\n\t" << endl;
}

//
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

//
void calculate(Connection *con)
{
    string onePackage;
    while (ParseOnePackage(con->inbuffer_, &onePackage))
    {
        string reqStr;
        if (!deLength(onePackage, &reqStr))
            return;
        cout << "去掉报头的正文：\n" << reqStr << std::endl;

        // 2.1 对请求Request，反序列化，得到一个结构化的请求对象
        Request req;
        if (!req.deserialize(reqStr))
            return;
        
        // 3.1 得到一个结构化的响应
        Response resp;
        cal(req, resp);

        // 4.1 得到了一个"字符串"
        string respStr;
        resp.serialize(&respStr);
        //cout << "计算完成, 序列化响应: " << respStr << endl;

        // 5.1 然后我们在发送响应，构建成为一个完整的报文
        con->outbuffer_ += enLength(respStr);
        //cout << "构建完成完整的响应\n" << send_string << endl;

        cout << "---------------result: " << con->outbuffer_ << endl;
    }
    // 发回去, 直接发
    if (con->sender_) 
        con->sender_(con);

    // // 如果没有发送完毕，需要对 对应的sock 开启写事件的关系，如果发完了，我们要关闭对写事件的关心
    // if (!con->outbuffer_.empty())
    //     con->tsp_->EnableReadWrite(con, true, true);
    // else 
    //     con->tsp_->EnableReadWrite(con, true, false);

}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }
    uint16_t port = atoi(argv[1]);

    // 创建一个堆上分配的 SelectServer 对象，并用 unique_ptr 智能指针管理它。
    unique_ptr<TcpServer> tsvr(new TcpServer(calculate, port));
    tsvr->InitServer();
    tsvr->Dispatch();

    return 0;
}