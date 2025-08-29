#include "udpServer.hpp"
#include <memory>
#include <unordered_map>
#include <fstream>
#include <signal.h>
#include <stdio.h>

using namespace std;
using namespace Server;


static void Usage(string proc)
{
    cout << "\nUsage:\n\t" << proc << " local_port\n\t" << endl;
}

// 处理client发来的数据(demo1)
void handlerMsg(int sockfd, string clientip, uint16_t clientport, string msg)
{
    string response_msg = msg;
    response_msg += " [Server echo]";

    // 把消息开始返回给客户端
    struct sockaddr_in client;
    bzero(&client, sizeof(client));

    client.sin_family = AF_INET;
    client.sin_port = htons(clientport);
    client.sin_addr.s_addr = inet_addr(clientip.c_str());

    sendto(sockfd, response_msg.c_str(), response_msg.size(), 0, (struct sockaddr*)&client, sizeof(client));
}


int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }
    uint16_t port = atoi(argv[1]);
    
    unique_ptr<udpServer> usvr(new udpServer(handlerMsg, port));

    usvr->initServer();
    usvr->startServer();

    return 0;
}