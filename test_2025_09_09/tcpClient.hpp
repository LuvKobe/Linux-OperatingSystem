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

#define NUM 1024

namespace client
{
    using namespace std;

    class TcpClient
    {
    public:
        TcpClient(const string &serverip, const uint16_t &serverport)
            : _sock(-1), _serverIp(serverip), _serverPort(serverport)
        {}

        // 初始化
        void initClient()
        {
            // 1. 创建套接字
            _sock = socket(AF_INET, SOCK_STREAM, 0);
            if (_sock < 0)
            {
                cerr << "socket create error" << endl;
            }
            cout << "_sock: " << _sock << endl; // 打印_sock文件描述符

            // 2. 客户端要bind绑定自己的网络信息
            // 但是这里不需要显示的bind, 这里由操作系统自己随机指定

            // 3. 客户端不需要listen

            // 4. 客户端不需要accept

            // 5. 要发起链接
        }

        // 启动
        void startClient()
        {
            // 5. 发起链接请求
            struct sockaddr_in server;
            memset(&server, 0, sizeof(server));
            server.sin_family = AF_INET;
            server.sin_port = htons(_serverPort);
            server.sin_addr.s_addr = inet_addr(_serverIp.c_str());

            int n = connect(_sock, (struct sockaddr*)&server, sizeof(server));
            if (0 != n) // 连接失败
            {
                cerr << "socket connect error" << endl;
            }
            else // 连接成功
            {   
                string msg;
                while (true)
                {
                    // 1. 先往服务端写消息
                    cout << "Enter# ";
                    getline(cin, msg);
                    write(_sock, msg.c_str(), msg.size());

                    // 2. 再从服务端读消息
                    char buffer[NUM];
                    int n = read(_sock, buffer, sizeof(buffer)-1);
                    if (n > 0) // 读取成功
                    {
                        // 把读到的数据当成字符串
                        buffer[n] = 0;
                        cout << "Server回显# " << buffer << endl;
                    }
                    else // 读取失败
                    {
                        break;
                    }
                }
            }
        }

        ~TcpClient()
        {
            // 文件描述符的生命周期随进程, 所以你的服务期进程如果退了, 这个没有close也不影响
            if(_sock >= 0) close(_sock);
        }

    private:
        int _sock;
        string _serverIp;
        uint16_t _serverPort;
    };
}