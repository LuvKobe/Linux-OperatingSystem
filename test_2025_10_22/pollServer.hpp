#pragma once

#include <iostream>
#include <functional>
#include <poll.h>
#include "sock.hpp"

namespace poll_ns
{
    static const int defaultPort = 8081;
    static const int num = 2048;
    static const int defaultfd = -1;

    using func_t = function<string (const string&)>;

    class PollServer
    {
    public:
        PollServer(func_t f, int port = defaultPort)
            : _func(f), _port(port), _listensock(-1), _rfds(nullptr)
        {
        }

        // 初始化服务器
        void initServer()
        {
            _listensock = Sock::Socket();
            Sock::Bind(_listensock, _port);
            Sock::Listen(_listensock);

            // 定义一个数组, 用来存储所有合法的文件描述符
            _rfds = new struct pollfd[num];
            for (int i = 0; i < num; i++)
            {
                ResetItem(i);
            }
            _rfds[0].fd = _listensock;
            _rfds[0].events = POLLIN;
        }

        // 打印数组中所有的fd
        void Print()
        {
            cout << "fd list: ";
            for (int i = 0; i < num; i++)
            {
                if (_rfds[i].fd != defaultfd)
                    cout << _rfds[i].fd << ", ";
            }
            cout << endl;
        }

        //
        void Accepter(int listensock)
        {
            // 那么select会告诉我, _listensock读事件就绪了
            string clientip;
            uint16_t clientport = 0;
            int sock = Sock::Accept(listensock, &clientip, &clientport);
            if (sock < 0)
                return;
            logMsg(NORMAL, "accept success [%s:%d]", clientip.c_str(), clientport);

            // 整个代码, 只有select有资格检测事件是否就绪
            // 所以我们要把新的sock托管交给select!
            // 托管的本质, 就是将sock, 添加到fdarray数组中即可!
            int i = 0;
            for (; i < num; i++)
            {
                // 合法的文件描述符就直接跳过
                if (_rfds[i].fd != defaultfd)
                    continue;
                else
                    break;
            }
            if (i == num)
            {
                logMsg(WARNING, "server is full, please wait");
                close(sock);
            }
            else
            {
                _rfds[i].fd = sock;
                _rfds[i].events = POLLIN;
                _rfds[i].revents = 0;
            }
            Print();
        }

        //
        void ResetItem(int i)
        {
            _rfds[i].fd = defaultfd;
            _rfds[i].events = 0;
            _rfds[i].revents = 0;
        }

        //
        void Recver(int pos)
        {
            // 1. 读取request
            char buffer[1024];
            ssize_t s = recv(_rfds[pos].fd, buffer, sizeof(buffer)-1, 0); 
            if (s > 0)
            {
                buffer[s] = 0;
                logMsg(NORMAL, "client# %s", buffer);
                //cout << "client# " << buffer << endl;
            }
            else if (s == 0)
            {
                close(_rfds[pos].fd);
                ResetItem(pos);
                logMsg(NORMAL, "client quit");
                return;
                //cout << "client quit" << endl;
            }
            else
            {
                close(_rfds[pos].fd);
                ResetItem(pos);
                logMsg(ERROR, "client quit: %s", strerror(errno));
                return;
                //cout << "client quit" << endl;
            }
            
            // 2. 处理request
            string response = _func(buffer);

            // write
            write(_rfds[pos].fd, response.c_str(), response.size());
        }

        // 处理对应的事件
        void HandlerReadEvent()
        {
            for (int i = 0; i < num; i++)
            {
                // 过滤点非法的fd
                if (_rfds[i].fd == defaultfd) 
                    continue;
                
                if (!(_rfds[i].events & POLLIN)) 
                    continue;

                // 正常的fd
                if (_rfds[i].fd == _listensock && (_rfds[i].revents & POLLIN))
                {
                    Accepter(_listensock);
                }
                else if(_rfds[i].revents & POLLIN)// 其他IO事件
                {
                    Recver(i);
                }
                else 
                {
                    // 什么都不做
                }
            }
        }


        // 启动服务器
        void start()
        {
            //int timeout = 0; // 非阻塞等待
            int timeout = -1; // 阻塞等待
            while (true)
            {
                int n = poll(_rfds, num, timeout);
                switch (n)
                {
                case 0:
                    logMsg(NORMAL, "timeout...");
                    break;
                case -1:
                    logMsg(WARNING, "poll error, code: %d, error msg: %s", errno, strerror(errno));
                    break;
                default:
                    // 此时说明有事情就绪了, 目前只有一个监听事件
                    logMsg(NORMAL, "have event ready...");
                    HandlerReadEvent(); // 处理请求
                    break;
                }
            }
        }

        ~PollServer()
        {
            if (_listensock < 0)
                close(_listensock);
            if (_rfds) delete []_rfds;
        }

    private:
        int _port;
        int _listensock;
        struct pollfd *_rfds;
        func_t _func;
    };
}
