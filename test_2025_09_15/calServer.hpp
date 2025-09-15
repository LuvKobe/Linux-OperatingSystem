#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <functional>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include "log.hpp"
#include "protocol.hpp"

namespace server
{
    enum
    {
        USAGE_ERR = 1,
        SOCKET_ERR,
        BIND_ERR,
        LISTEN_ERR
    };

    static const uint16_t gport = 8080;
    static const int gbacklog = 5;

    // const Request &req: 输入型
    // Response &resp: 输出型
    typedef function<bool(const Request &req, Response &resp)> func_t;

    // 保证解耦
    void handlerEntery(int sock, func_t func)
    {
        std::string inbuffer;
        while (true)
        {
            // 1. 读取："content_len"\r\n"x op y"\r\n
            // 1.1 你怎么保证你读到的消息是 【一个】完整的请求
            string req_text, req_str;
            // 1.2 我们保证，我们req_text里面一定是一个完整的请求："content_len"\r\n"x op y"\r\n
            if (!recvPackage(sock, inbuffer, &req_text))
                return;
            cout << "带报头的请求：\n" << req_text << std::endl;
            if (!deLength(req_text, &req_str))
                return;
            cout << "去掉报头的正文：\n" << req_str << std::endl;
            

            // 2. 对请求Request，反序列化
            // 2.1 得到一个结构化的请求对象
            Request req;
            if (!req.deserialize(req_str))
                return;

            // 3. 计算机处理，req.x, req.op, req.y --- 业务逻辑
            // 3.1 得到一个结构化的响应
            Response resp;
            func(req, resp); // req的处理结果，全部放入到了resp， 回调是不是不回来了？不是！

            // 4.对响应Response，进行序列化
            // 4.1 得到了一个"字符串"
            string resp_str;
            resp.serialize(&resp_str);

            cout << "计算完成, 序列化响应: " <<  resp_str << endl;

            // 5. 然后我们在发送响应
            // 5.1 构建成为一个完整的报文
            string send_string = enLength(resp_str);
            cout << "构建完成完整的响应\n" <<  send_string << endl;
        
            send(sock, send_string.c_str(), send_string.size(), 0); // 其实这里的发送也是有问题的，不过后面再说
        }
    }

    class CalServer
    {
    public:
        CalServer(const uint16_t &port = gport) : _listensock(-1), _port(port)
        {
        }
        void initServer()
        {
            // 1. 创建socket文件套接字对象
            _listensock = socket(AF_INET, SOCK_STREAM, 0);
            if (_listensock < 0)
            {
                logMsg(FATAL, "create socket error");
                exit(SOCKET_ERR);
            }
            logMsg(NORMAL, "create socket success: %d", _listensock);

            // 2. bind绑定自己的网络信息
            struct sockaddr_in local;
            memset(&local, 0, sizeof(local));
            local.sin_family = AF_INET;
            local.sin_port = htons(_port);
            local.sin_addr.s_addr = INADDR_ANY;
            if (bind(_listensock, (struct sockaddr *)&local, sizeof(local)) < 0)
            {
                logMsg(FATAL, "bind socket error");
                exit(BIND_ERR);
            }
            logMsg(NORMAL, "bind socket success");

            // 3. 设置socket 为监听状态
            if (listen(_listensock, gbacklog) < 0) // 第二个参数backlog后面在填这个坑
            {
                logMsg(FATAL, "listen socket error");
                exit(LISTEN_ERR);
            }
            logMsg(NORMAL, "listen socket success");
        }
        void startServer(func_t func)
        {
            for (;;)
            {
                // 4. server 获取新链接
                // sock, 和client进行通信的fd
                struct sockaddr_in peer;
                socklen_t len = sizeof(peer);
                int sock = accept(_listensock, (struct sockaddr *)&peer, &len);
                if (sock < 0)
                {
                    logMsg(ERROR, "accept error, next");
                    continue;
                }
                logMsg(NORMAL, "accept a new link success, get new sock: %d", sock); // ?

                // version 2 多进程版(2)
                pid_t id = fork();
                if (id == 0) // child
                {
                    close(_listensock);
                    // if(fork()>0) exit(0);
                    //  serviceIO(sock);
                    handlerEntery(sock, func);
                    close(sock);
                    exit(0);
                }
                close(sock);

                // father
                pid_t ret = waitpid(id, nullptr, 0);
                if (ret > 0)
                {
                    logMsg(NORMAL, "wait child success"); // ?
                }
            }
        }
        ~CalServer() {}

    private:
        int _listensock; // 不是用来进行数据通信的，它是用来监听链接到来，获取新链接的！
        uint16_t _port;
    };

} // namespace server
