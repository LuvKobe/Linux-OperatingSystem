#include "epollServer.hpp"
#include <memory>

//using namespace std;
using namespace epoll_ns;

// 手册
static void Usage(string proc)
{
    cout << "\nUsage:\n\t" << proc << " port\n\t" << endl;
}

string echo(const string &message)
{
    return "I am epollServer, " + message;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }
    uint16_t port = atoi(argv[1]);

    //创建一个堆上分配的 SelectServer 对象，并用 unique_ptr 智能指针管理它。
    unique_ptr<epollServer> epollsvr(new epollServer(echo, port));
    epollsvr->initServer();
    epollsvr->start();

    return 0;
}