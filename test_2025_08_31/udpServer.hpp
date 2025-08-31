#pragma once

#include <iostream>
#include <string>
#include <strings.h>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <functional>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
/*

int socket(int domain, int type, int protocol); 创建套接字字，就类似于打开网卡

domain: 想本地通信还是网络通信

type: 我们的socket提供的能力类型(提供流式服务？还是用户数据包服务？)

protocol: 可以默认为0
*/

using namespace std;

namespace Server
{
    using namespace std;
    static const string defaultIp = "0.0.0.0"; // 默认IP
    static const int gnum = 1024; // 缓冲区大小
    // 错误码
    enum 
    {
        USAGE_ERR = 1,
        SOCKET_ERR = 2,
        BIND_ERR = 3,
        OPEN_ERR = 4
    };

    typedef function< void(int, string, uint16_t, string) > func_t; // 函数对象类型, 做回调用的

    class udpServer
    {
    public:
        udpServer(const func_t& cb, const uint16_t &port, const string &ip = defaultIp)
            : _callback(cb), _port(port), _ip(ip), _sockfd(-1)
        {}

        
        void initServer()
        {
            // 1. 创建套接字()
        /*
            IP+端口最后会被整合进内核的相关的协议栈当中，然后最终以文件的形式呈现出来。
            所以我们上层在编程的时候用的是这个socket, 其实就是一个文件描述符。
            它的文件描述符编号是 3(因为0、1、2被占用了)
        */
            _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
            if (-1 == _sockfd) // 创建失败
            {
                cerr << "socket error: " << errno << " : " << strerror(errno) << endl;
                exit(SOCKET_ERR);
            }
            cout << "socket success: " << _sockfd << endl; // 创建成功

            // 2. 绑定 ip + port (需要band绑定我们自己设定的IP和端口设置进我们的系统当中)
            // 未来服务器要明确的port, 不能随意改变
            struct sockaddr_in local; // 在用户栈上定义了一个结构体变量
            bzero(&local, sizeof(local));
            local.sin_family = AF_INET;
            local.sin_port = htons(_port); // 先识别是大端还是小端, 如果是小端就要转成大端

            // a.将字符串分割的IP地址转成整数(string转uint32_t)
            // b.将整数转成网络序列放到local里(htonl())
            local.sin_addr.s_addr = inet_addr(_ip.c_str()); 
            //local.sin_addr.s_addr = INADDR_ANY; // 任意地址绑定(服务器的真是写法)

            
            int n = bind(_sockfd, (struct sockaddr*)&local, sizeof(local));
            if (-1 == n)
            {
                cerr << "bind error: " << errno << " : " << strerror(errno) << endl;
                exit(BIND_ERR);
            }
        }

        //
        void startServer()
        {
            // 服务器的本质其实就是一个死循环
            char buffer[gnum];
            for (;;)
            {
                struct sockaddr_in peer;  
                socklen_t len = sizeof(peer);
                // 从套接字_sockfd中去读数据,然后放到buffer中
                ssize_t s = recvfrom(_sockfd, buffer, sizeof(buffer) - 1, 0 , (struct sockaddr*)&peer, &len);
                // 1.数据是什么 2.谁发的
                if (s > 0)
                {
                    buffer[s] = 0;
                    string clientIp = inet_ntoa(peer.sin_addr); // 1. 网络序列
                    uint16_t clientPort = ntohs(peer.sin_port); 
                    string msg = buffer;

                    // 打印数据
                    cout << clientIp << "[" << clientPort << "]# " << msg << endl;

                    // 我们把数据读上来以后, 要对数据做处理
                    _callback(_sockfd, clientIp, clientPort, msg);
                }
            }
        }

        ~udpServer()
        {
            sleep(1);
        }

    private:
        uint16_t _port; // 服务器需要绑定的端口号
        string _ip;     // 服务器的IP地址
        int _sockfd;    // 套接字
        func_t _callback; // 回调方法
    };
}
