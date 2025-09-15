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
#include "protocol.hpp"

#define NUM 1024

namespace client
{
    using namespace std;

    class CalClient
    {
    public:
        CalClient(const string &serverip, const uint16_t &serverport)
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
                exit(2);
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
                string line;
                string inbuffer;
                while (true)
                {
                    cout << "mycal>>> ";
                    getline(std::cin, line);  // 1+1
                    Request req = ParseLine(line); // "1+1"
                    string content;
                    req.serialize(&content);
                    string send_string = enLength(content);
                    cout << "sendstring:\n"
                              << send_string << endl;
                    send(_sock, send_string.c_str(), send_string.size(), 0); // bug?? 不管

                    string package, text;
                    //  "content_len"\r\n"exitcode result"\r\n
                    if (!recvPackage(_sock, inbuffer, &package))
                        continue;
                    if (!deLength(package, &text))
                        continue;
                    // "exitcode result"
                    Response resp;
                    resp.deserialize(text);
                    cout << "exitCode: " << resp.exitcode << std::endl;
                    cout << "result: " << resp.result << std::endl;
                }
            }
        }

        Request ParseLine(const string &line)
        {
            // "1+1" "123*456" "12/0"

            // 0: 操作符之前, 1:碰到了操作符, 2:操作符之后
            int status = 0;
            int i = 0;
            int cnt = line.size();
            string left, right;
            char op;
            while (i < cnt)
            {
                switch (status)
                {
                case 0:
                {
                    if (!isdigit(line[i]))
                    {
                        op = line[i];
                        status = 1;
                    }
                    else
                        left.push_back(line[i++]);
                }
                break;
                case 1:
                    i++;
                    status = 2;
                    break;
                case 2:
                    break;
                default:
                    right.push_back(line[i++]);
                    break;
                }
            }
            //cout << stoi(left) << " " << stoi(right) << " " << op << endl;
            return Request(stoi(left), stoi(right), op);
        }

        ~CalClient()
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