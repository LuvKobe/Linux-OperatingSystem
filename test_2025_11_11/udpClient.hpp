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
            char cmdline[2014];
            while (!_quit)
            {
                // 蓝色粗体提示
                cout << "\033[1;34mPlease Enter# \033[0m";
                // cin >> msg;
                fgets(cmdline, sizeof(cmdline), stdin);
                msg = cmdline;

                // 发送数据给Server
                sendto(_sockfd, msg.c_str(), msg.size(), 0, (struct sockaddr*)&server, sizeof(server));


                // 接受数据
                char buffer[1024];
                struct sockaddr_in temp;
                socklen_t temp_len = sizeof(temp);
                size_t n = recvfrom(_sockfd, buffer, sizeof(buffer)-1, 0, (struct sockaddr*)&temp, &temp_len);
                if (n >= 0) buffer[n] = 0;
                // 绿色输出用户输入的内容
                cout << "\033[32m服务器的翻译结果# \n" << buffer << endl;
            }
        }

        ~udpClient()
        {}

    private:
        int _sockfd; // 套接字
        string _serverIp; // 需要访问的服务器ip
        uint16_t _serverPort; // 需要访问的服务器端口号
        bool _quit;
    };
}