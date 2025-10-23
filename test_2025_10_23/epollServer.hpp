#pragma once

#include <iostream>
#include <cstdio>
#include <string>
#include <functional>
#include <sys/epoll.h>
#include "err.hpp"
#include "log.hpp"
#include "sock.hpp"

#define GREEN "\033[32m" // 绿色
#define RED "\033[31m" // 红色
#define RESET "\033[0m" // 重置颜色为默认

namespace epoll_ns
{
    class epollServer
    {
        static const int defaultport = 8888;
        static const int size = 256;
        static const int defaultvalue = -1;
        static const int defaultnum = 64;
        using func_t = function<string (const string&)>;
    public:
        epollServer(func_t f, uint16_t port = defaultport, int num = defaultnum)
            :_func(f), _port(port), _listensock(-1), _epfd(-1), _revs(nullptr), _num(num)
        {}

        //
        void initServer()
        {
            // 1. 创建socket
            _listensock = Sock::Socket();
            Sock::Bind(_listensock, _port);
            Sock::Listen(_listensock);

            // 2. 创建epoll模型
            _epfd = epoll_create(size);
            if (_epfd < 0)
            {
                logMsg(FATAL, "epoll create error: %s", strerror(errno));
                exit(EPOLL_CREATE_ERR);
            }

            // 3. 添加listensock到epoll中
            // 将【读事件】添加到内核的红黑树结点中 
            struct epoll_event ev;
            ev.events = EPOLLIN;
            ev.data.fd = _listensock; // 当事件就绪, 被重新捞取上来的时候, 我们要知道是哪一个fd就绪了 
            epoll_ctl(_epfd, EPOLL_CTL_ADD, _listensock, &ev);

            // 4. 申请就绪事件的空间
            _revs = new struct epoll_event[_num];

            logMsg(NORMAL, "init server success");
        }

        // 
        void HandlerEvent(int readyNum)
        {
            logMsg(DEBUG, "%sHandlerEvent start...%s", GREEN, RESET);
            for (int i = 0; i < readyNum; i ++)
            {
                // 开始处理就绪事件
                uint32_t events = _revs[i].events;
                int sock = _revs[i].data.fd;

                if (sock == _listensock && (events & EPOLLIN))
                {
                    // _listen套接字的读事件就绪, 那么就获取新链接
                    string clientip;
                    uint16_t clientport;
                    int fd = Sock::Accept(sock, &clientip, &clientport);
                    if (fd < 0)
                    {
                        logMsg(WARNING, "accept error");
                        continue;
                    }
                    // 获取fd成功, 可以直接读取吗？？？肯定不可以！要放入epoll模型中
                    struct epoll_event ev;
                    ev.events = EPOLLIN;
                    ev.data.fd = fd;
                    epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev);

                }
                else if (sock != _listensock && (events & EPOLLIN))
                {
                    // 普通的读事件就绪
                    char buffer[1024]; 
                    int n = recv(sock, buffer, sizeof(buffer), 0);
                    if (n > 0)
                    {
                        buffer[n] = 0;
                        logMsg(NORMAL, "client# %s", buffer);

                        // TODO
                        string response = _func(buffer);
                        send(sock, response.c_str(), response.size(), 0);
                    }
                    else if (n == 0)
                    {
                        // 建议先把事件从epoll中移除, 再关闭fd
                        epoll_ctl(_epfd, EPOLL_CTL_DEL, sock, nullptr); 
                        close(sock);
                        logMsg(NORMAL, "client quit....");
                    }
                    else  
                    {
                        epoll_ctl(_epfd, EPOLL_CTL_DEL, sock, nullptr); 
                        close(sock);
                        logMsg(ERROR, "recv error, code: %d, errstring: %s", errno, strerror(errno));
                    }
                }
                else  
                {
                    // TODO
                }

            }
            logMsg(DEBUG, "%sHandlerEvent end...%s", RED, RESET);
        }

        //
        void start()
        {
            //int timeout = 1000; // 每隔 1s 一次
            //int timeout = 0; // 非阻塞等待
            int timeout = -1; // 永久阻塞
            while (true)
            {
                int n = epoll_wait(_epfd, _revs, _num, timeout); // n里面存储的是就绪事件的个数
                switch (n)
                {
                case 0:
                    logMsg(NORMAL, "timeout...");
                    break;
                case -1:
                    logMsg(WARNING, "epoll_wait failed, code: %d, errstring: %s", errno, strerror(errno));
                    break;
                default:
                    logMsg(NORMAL, "have a event ready!!!"); // 已经有事件就绪了, 那么就需要去处理
                    HandlerEvent(n);
                    break;
                }
            }
        }

        ~epollServer()
        {
            if (_listensock != defaultvalue) close(_listensock);
            if (_epfd != defaultvalue) close(_epfd);
            if (_revs) delete []_revs;
        }
    private:
        uint16_t _port;
        int _listensock;
        int _epfd;
        struct epoll_event *_revs;
        int _num; 
        func_t _func; // 数据处理函数
    };
}