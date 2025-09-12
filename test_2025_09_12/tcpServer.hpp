#pragma once 

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>


#include "log.hpp"
#include "Task.hpp"
#include "ThreadPool.hpp"   


namespace server
{
    using namespace std;

    // 错误码
    enum 
    {
        USAGE_ERR = 1,
        SOCKET_ERR = 2,
        BIND_ERR = 3,
        LISTEN_ERR = 4,
        ACCEPT_ERR = 5
    };

    static const uint16_t gport = 8080;
    static const int gbacklog = 5;
    class TcpServer; // 声明

    class ThreadData
    {
    public:
        ThreadData(TcpServer *self, int sock) 
            :_self(self), _sock(sock)
        {}
    public:
        TcpServer *_self;
        int _sock;
    };

    class TcpServer
    {
    public:
        TcpServer(const uint16_t &port = gport) 
            : _listensock(-1), _port(port)
        {}

        // 初始化
        void initServer()
        {
            // 1. 创建socket文件套接字对象
            _listensock = socket(AF_INET, SOCK_STREAM, 0);
            if (_listensock < 0)
            {
                logMsg(FATAL, "create socket error");
                exit(SOCKET_ERR);
            }
            logMsg(NORMAL, "create socket success: %d", _listensock); // 打印_listensock文件描述符

            // 2. bind绑定自己的网络信息
            struct sockaddr_in local;
            memset(&local, 0, sizeof(local));
            local.sin_family = AF_INET;
            local.sin_port = htons(_port); // htons是主机转网络
            local.sin_addr.s_addr = INADDR_ANY; // 服务端接收任意IP地址
            int b = bind(_listensock, (struct sockaddr*)&local, sizeof(local));
            if (b < 0)
            {
                logMsg(FATAL, "bind socket error");
                exit(BIND_ERR); // 绑定失败
            }
            logMsg(NORMAL, "bind socket success");

            // 3. 设置socket为监听状态(TCP是面向链接的, 所以必须要listen)
            int n = listen(_listensock, gbacklog);
            if (n < 0)
            {
                logMsg(FATAL, "listen socket error");
                exit(LISTEN_ERR);
            }
            logMsg(NORMAL, "listen socket success");

        }

        // 启动
        void startServer()
        {
            // 线程池初始化
            ThreadPool<Task>::getInstance()->run();
            logMsg(NORMAL, "Thread init success");
            // 死循环
            for ( ; ; )
            {
                // 4. server获取新链接
                // sock是未来和client客户端进行通信的
                struct sockaddr_in peer;
                socklen_t len = sizeof(peer);
                int sock = accept(_listensock, (struct sockaddr*)&peer, &len);
                if (sock < 0)
                {
                    logMsg(ERROR, "accept error, next continue");
                    continue;
                }
                logMsg(NORMAL, "accept a new link success, get new sock: %d", sock); // 获取链接成功

                // 5. 接下来开始通信, 使用上面的sock
                // version 4 : 线程池版
                ThreadPool<Task>::getInstance()->Push(Task(sock, serviceIO));
            }
        }

        ~TcpServer()
        {}
    private:
        int _listensock;  // 这个套接字不是用来通信的, 它是用来监听链接到来, 获取新链接的.
        uint16_t _port;
    };
}