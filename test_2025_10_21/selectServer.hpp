#pragma once

#include <iostream>
#include <functional>
#include "sock.hpp"

namespace select_ns
{
    static const int defaultPort = 8081;
    static const int fdnum = sizeof(fd_set) * 8;
    static const int defaultfd = -1;

    using func_t = function<string (const string&)>;

    class SelectServer
    {
    public:
        SelectServer(func_t f, int port = defaultPort)
            : func(f), _port(port), _listensock(-1), fdarray(nullptr)
        {
        }

        // 初始化服务器
        void initServer()
        {
            _listensock = Sock::Socket();
            Sock::Bind(_listensock, _port);
            Sock::Listen(_listensock);

            // 定义一个数组, 用来存储所有合法的文件描述符
            fdarray = new int[fdnum];
            for (int i = 0; i < fdnum; i++)
                fdarray[i] = defaultfd;
            fdarray[0] = _listensock;
        }

        // 打印数组中所有的fd
        void Print()
        {
            cout << "fd list: ";
            for (int i = 0; i < fdnum; i++)
            {
                if (fdarray[i] != defaultfd)
                    cout << fdarray[i] << ", ";
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
            for (; i < fdnum; i++)
            {
                // 合法的文件描述符就直接跳过
                if (fdarray[i] != defaultfd)
                    continue;
                else
                    break;
            }
            if (i == fdnum)
            {
                logMsg(WARNING, "server is full, please wait");
                close(sock);
            }
            else
            {
                fdarray[i] = sock;
            }
            Print();
        }

        //
        void Recver(int sock, int pos)
        {
            // 1. 读取request
            char buffer[1024];
            ssize_t s = recv(sock, buffer, sizeof(buffer)-1, 0); 
            if (s > 0)
            {
                buffer[s] = 0;
                logMsg(NORMAL, "client# %s", buffer);
                //cout << "client# " << buffer << endl;
            }
            else if (s == 0)
            {
                close(sock);
                fdarray[pos] = defaultfd;
                logMsg(NORMAL, "client quit");
                return;
                //cout << "client quit" << endl;
            }
            else
            {
                close(sock);
                fdarray[pos] = defaultfd;
                logMsg(ERROR, "client quit: %s", strerror(errno));
                return;
                //cout << "client quit" << endl;
            }
            
            // 2. 处理request
            string response = func(buffer);

            // write
            write(sock, response.c_str(), response.size());
        }

        // 处理对应的事件
        void HandlerReadEvent(fd_set &rfds)
        {
            for (int i = 0; i < fdnum; i++)
            {
                // 过滤点非法的fd
                if (fdarray[i] == defaultfd) continue;
                
                // 正常的fd
                if (FD_ISSET(fdarray[i], &rfds) && fdarray[i] == _listensock) // 如果_listensock在rfds集合中
                {
                    Accepter(_listensock);
                }
                else if(FD_ISSET(fdarray[i], &rfds))// 其他IO事件
                {
                    Recver(fdarray[i], i);
                }
                else 
                {
                    //
                }
            }
        }



        // 启动服务器
        void start()
        {
            while (true)
            {
                // 阻塞IO的写法
                // string clientip;
                // uint16_t clientport = 0;
                // int sock = Sock::Accept(_listensock, &clientip, &clientport);
                // if (sock < 0) continue;
                
                // -----------------------------------------------------------------

                // IO多路转接的写法
                // 读的文件描述符集合
                fd_set rfds;                // 定义集合
                FD_ZERO(&rfds);             // 清空集合, 并初始化

                // 一般而言, 要使用select的话, 需要程序员自己维护一个保存所有合法fd的数组
                int maxfd = fdarray[0];
                for (int i = 0; i < fdnum; i++) // 所有的合法的文件描述符全部被放进了整个数组
                {
                    if (fdarray[i] == defaultfd) 
                        continue;
                    // 下面都是合法的文件描述符
                    FD_SET(fdarray[i], &rfds); // 把合法的文件描述符全部添加到集合中

                    if (maxfd < fdarray[i]) // 更新所有fd中最大的fd
                        maxfd = fdarray[i];
                }
                logMsg(NORMAL, "max fd is: %d", maxfd);
                
                //struct timeval timeout = {3, 0};
                //int n = select(_listensock+1, &rfds, nullptr, nullptr, &timeout); // 只关心读事件
                int n = select(maxfd+1, &rfds, nullptr, nullptr, nullptr); // 阻塞式等待
                switch (n)
                {
                case 0:
                    logMsg(NORMAL, "timeout...");
                    break;
                case -1:
                    logMsg(WARNING, "select error, code: %d, error msg: %s", errno, strerror(errno));
                    break;
                default:
                    // 此时说明有事情就绪了, 目前只有一个监听事件
                    logMsg(NORMAL, "have event ready...");
                    HandlerReadEvent(rfds); // 处理读事件
                    //HandlerWriteEvent(rfds); // 处理写事件
                    break;
                }
                //sleep(1);
                // 开始进行服务器的处理逻辑
            }
        }

        ~SelectServer()
        {
            if (_listensock < 0)
                close(_listensock);
            if (fdarray) delete []fdarray;
        }

    private:
        int _port;
        int _listensock;
        int *fdarray; // 自己维护一个数组
        func_t func;
    };
}
