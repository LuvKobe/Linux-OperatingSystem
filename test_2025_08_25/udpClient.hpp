#pragma once

#include <iostream>
#include <string>
#include <strings.h>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

namespace Client
{
    using namespace std;
    enum
    {
        USAGE_ERR = 1,
        SOCKET_ERR = 2,
        BIND_ERR
    };
    class udpClient
    {
    public:
        udpClient(const string &serverip, const uint16_t &serverport)
            : _sockfd(-1), _serverIp(serverip), _serverPort(serverport), _quit(false)
        {}

        //
        void initClient()
        {
            // 1. 创建socket
            _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
            if (-1 == _sockfd)
            {
                cerr << "socket error: " << errno << " : " << strerror(errno) << endl;
                exit(SOCKET_ERR);
            }
            cout << "socket success: " << _sockfd << endl;


            // 2. client要不要bind[必须要的], client要不要显示的bind？要不要程序员自己bind？[不需要]
            // 写服务器的是一家公司, 写client的是无数家公司 --> OS有自动形成端口进行bind！ 
        }

        //
        void runClient()
        {
            struct sockaddr_in server;
            memset(&server, 0, sizeof(server));
            server.sin_family = AF_INET;
            server.sin_addr.s_addr = inet_addr(_serverIp.c_str());
            server.sin_port = htons(_serverPort);

            string msg;
            while (!_quit)
            {
                cout << "Please Enter# ";
                cin >> msg;

                // 发送数据给Server
                sendto(_sockfd, msg.c_str(), msg.size(), 0, (struct sockaddr*)&server, sizeof(server));

            }
        }

        ~udpClient()
        {
        }

    private:
        int _sockfd;
        string _serverIp;
        uint16_t _serverPort;
        bool _quit;
    };
}