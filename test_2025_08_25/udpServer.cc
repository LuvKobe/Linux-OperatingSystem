#include "udpServer.hpp"
#include <memory>

using namespace std;
using namespace Server;

static void Usage(string proc)
{
    cout << "\nUsage:\n\t" << proc << " local_port\n\t" << endl;
}

// 处理client发来的数据
void handlerMsg(string clientip, uint16_t clientport, string msg)
{
    // 对msg进行特定的业务处理, 而不关心msg怎么来的
    // 故完成了server通信和业务逻辑的解耦
}

/*
./udpServer ip port

./udpServer 172.19.61.204(绑定服务器的内网ip) 8080

sudo netstat -nuap
*/
int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }
    uint16_t port = atoi(argv[1]);
    //string ip = argv[1];
    
    unique_ptr<udpServer> usvr(new udpServer(handlerMsg, port));

    usvr->initServer();
    usvr->startServer();

    return 0;
}