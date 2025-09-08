#include "tcpClient.hpp"
#include <memory>
#include <string>

using namespace client;
using namespace std;

// 手册
static void Usage(string proc)
{
    cout << "\nUsage:\n\t" << proc << " local_port\n\t" << endl;
}

// ./tcpclient serverip serverport
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(1);
    }
    // 拿到目标服务器的ip+port
    string serverip = argv[1];      // 第一个参数是ip
    uint16_t serverport = atoi(argv[2]);    // 第二个参数是端口号port

    unique_ptr<TcpClient> tcli(new TcpClient(serverip, serverport));
    tcli->initClient();
    tcli->startClient();
    
    return 0;
}