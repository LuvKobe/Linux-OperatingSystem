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
#include <sys/wait.h>
#include "log.hpp"

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
            logMsg(NORMAL, "create socket success");
            cout << "_listensock: " << _listensock << endl; // 打印_listensock文件描述符

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
            // 告诉操作系统自动忽略 SIGCHLD 信号，从而让父进程不需要显式调用 wait() 来回收子进程资源。
            // 这样，操作系统会在子进程结束后自动清理相关资源，防止出现僵尸进程。
            signal(SIGCHLD, SIG_IGN);

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
                logMsg(NORMAL, "accept a new link success"); // 获取链接成功
                cout << "sock: " << sock << endl; // sock=3

                // 5. 接下来开始通信, 使用上面的sock
                //serviceIO(sock); // version 1

                // version 2 : 多进程版(2个)
                pid_t id = fork();
                if (0 == id) // child
                {
                    close(_listensock); // 关闭掉不需要的文件描述符
                    //if (fork() > 0) exit(0);
                    serviceIO(sock); 
                    close(sock);
                    exit(0);
                }

                // 父进程关掉不需要的文件描述符
                close(sock);
                
                // father进行等待回收子进程
                // pid_t ret = waitpid(id, nullptr, 0); // 设置为阻塞式等待
                // if (ret > 0)
                // {
                //     cout << "wait success: " << ret << endl;
                // }
                
            }
        }

        // 进行通信的读写(第一版)
        void serviceIO(int sock)
        {
            char buffer[1024];
            while (true)
            {
                ssize_t n = read(sock, buffer, sizeof(buffer)-1);
                if (n > 0) // 读取成功
                {
                    // 把读到的数据当成字符串
                    buffer[n] = 0; // 把字符串结尾的'\n'设置为0
                    cout << "recv msg: " << buffer << endl;

                    // 把读过来的消息再转发回去
                    string outbuffer = buffer;
                    outbuffer += " server[echo]";
                    write(sock, outbuffer.c_str(), outbuffer.size());
                }
                else if (n == 0)
                {
                    // 代表client退出
                    logMsg(NORMAL, "client quit, me too!");
                    break;
                }
            }
        }

        ~TcpServer()
        {}
    private:
        int _listensock;  // 这个套接字不是用来通信的, 它是用来监听链接到来, 获取新链接的.
        uint16_t _port;
    };
}