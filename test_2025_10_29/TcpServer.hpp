#pragma once 

#include <iostream>
#include <cassert>
#include <functional>
#include <unordered_map> 
#include "Sock.hpp"
#include "Log.hpp"
#include "Err.hpp"
#include "Epoller.hpp"
#include "Util.hpp"
#include "Protocol.hpp"

using namespace std;

namespace tcpserver
{
    class Connection;
    class TcpServer;

    static const uint16_t defaultport = 8888;
    static const int num = 64;

    using func_t = function<void(Connection *)>;
    //using hander_t = function<void(const string &package)>;

    // 把每一个Connection都看作成一个套接字
    class Connection
    {
    public:
        Connection(int sock, TcpServer *tsp)
            : sock_(sock), tsp_(tsp)
        {}

        // 注册方法
        void Register(func_t r, func_t s, func_t e)
        {
            recver_ = r;
            sender_ = s;
            excepter_ = e;
        }

        ~Connection()
        {}

        void Close()
        {
            close(sock_);
        }
    public:
        int sock_;
        string inbuffer_;  // 和sock_套接字对应的输入缓冲区
        string outbuffer_; // 和sock_套接字对应的输出缓冲区

        func_t recver_;   // 从sock_中读
        func_t sender_;   // 从sock_中写
        func_t excepter_; // 处理sock_IO的时候上面的异常事件

        TcpServer *tsp_; // ???

        uint64_t lasttime;
    };

    class TcpServer
    {
    private:
        // 读取事件
        void Recver(Connection *con)
        {
            con->lasttime = time(nullptr);

            char buffer[1024];
            while (true)
            {
                ssize_t s = recv(con->sock_, buffer, sizeof(buffer)-1, 0);
                if (s > 0)
                {
                    buffer[s] = 0;
                    con->inbuffer_ += buffer; // 将读到的数据入队列
                    logMsg(DEBUG, "\n%s", con->inbuffer_.c_str());
                    _service(con);
                }
                else if (s == 0)
                {
                    if (con->excepter_) 
                    {
                        con->excepter_(con);
                        return;
                    }
                }
                else  
                {
                    if (errno == EAGAIN || errno == EWOULDBLOCK)
                        break;
                    else if (errno == EINTR)
                        continue;
                    else
                    {
                        if (con->excepter_) 
                        {
                            con->excepter_(con);
                            return;
                        }
                    }
                }
            } // while
            
        }

        // 发送处理
        void Sender(Connection *con)
        {
            con->lasttime = time(nullptr);

            while (true)
            {
                ssize_t s = send(con->sock_, con->outbuffer_.c_str(), con->outbuffer_.size(), 0);
                if (s > 0)
                {
                    if (con->outbuffer_.empty()) 
                    {
                        //EnableReadWrite(con, true, false);
                        break;
                    }
                    else 
                        con->outbuffer_.erase(0, s);
                }
                else  
                {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) 
                        break;
                    else if (errno == EINTR) 
                        continue;
                    else  
                    {
                        if (con->excepter_) 
                        {
                            con->excepter_(con);
                            return;
                        }
                    }
                }
            }

            // 如果没有发送完毕，需要对 对应的sock 开启写事件的关系，如果发完了，我们要关闭对写事件的关心
            if (!con->outbuffer_.empty())
                con->tsp_->EnableReadWrite(con, true, true);
            else
                con->tsp_->EnableReadWrite(con, true, false);
        }

        // 异常处理
        void Excepter(Connection *con)
        {
            logMsg(DEBUG, "Excepter begin");
            _epoller.Control(con->sock_, 0, EPOLL_CTL_DEL);
            con->Close();
            _connects.erase(con->sock_);

            logMsg(DEBUG, "关闭 %d 文件描述符的所有资源", con->sock_);
            delete con;
        }

        //
        void Accepter(Connection *conn)
        {
            for (;;)
            {
                // 获取链接
                string clientip;
                uint16_t clientport;
                int err = 0;
                int sock = _sock.Accept(&clientip, &clientport, &err);
                if (sock > 0)
                {
                    // 拿到了链接
                    AddConnection(sock, EPOLLIN | EPOLLET,
                                  bind(&TcpServer::Recver, this, std::placeholders::_1),
                                  bind(&TcpServer::Sender, this, std::placeholders::_1),
                                  bind(&TcpServer::Excepter, this, std::placeholders::_1));
                    logMsg(DEBUG, "get a new link, info: [%s : %d]", clientip.c_str(), clientport);
                }
                else
                {
                    if (err == EAGAIN || err == EWOULDBLOCK)
                        break;
                    else if (err == EINTR)
                        continue;
                    else
                        break;
                }
            }
        }

        // 添加链接
        void AddConnection(int sock, uint32_t events, func_t recver, func_t sender, func_t excepter)
        {
            // 1. 首先要为该sock创建Connection, 并初始化, 并添加到_connects中
            // 我们创建出来的文件描述符是【阻塞】的, 但是在ET模式下, 要把对应的文件描述符设置为【非阻塞】
            if (events & EPOLLET) 
                Util::SetNonBlock(_sock.Fd()); // 把fd设置为非阻塞, 然后再把它添加到epoll里, 以ET的模式来工作
            Connection *conn = new Connection(sock, this);

            // 2. 给对应的sock设置对应的回调处理方法
            conn->Register(recver, sender, excepter);
            
            // 3. 其次将sock与它要关心的事件 "写透式" 的注册到epoll中, 让epoll帮我们关心
            bool r = _epoller.AddEvent(sock, events);
            assert(r);
            (void)r;

            // 4. 将map里面的K&V添加到_connects对象中
            _connects.insert(pair<int, Connection*>(sock, conn));

            logMsg(DEBUG, "add new sock[%d] in epoll and unordered_map", sock);
        }

        // 判断是否存在
        bool IsConnectionExists(int sock)
        {
            auto iter = _connects.find(sock);
            return iter != _connects.end();
        }

        //
        void Loop(int timeout)
        {
            int n = _epoller.Wait(_revs, _num, timeout); // 获取已经就绪的事件
            for (int i = 0; i < n; i++)
            {
                int sock = _revs[i].data.fd; // 已经就绪的文件描述符
                uint32_t events = _revs[i].events; // 已经就绪的事件

                // 将所有的异常问题, 全部转化为, 读写问题
                if (events & EPOLLERR) 
                    events |= (EPOLLIN | EPOLLOUT);   
                if (events & EPOLLHUP) 
                    events |= (EPOLLIN | EPOLLOUT);
                
                // listen事件就绪
                // 1.读事件就绪
                // 2.套接字所对应的connection对象存在
                // 3.connection对象所对应的receiver对象也存在
                if ((events & EPOLLIN) && IsConnectionExists(sock) &&  _connects[sock]->recver_)
                {
                    _connects[sock]->recver_(_connects[sock]);
                }

                // 1.写事件就绪
                // 2.套接字所对应的connection对象存在
                // 3.connection对象所对应的receiver对象也存在
                if ((events & EPOLLOUT) && IsConnectionExists(sock) &&  _connects[sock]->sender_)
                {
                    _connects[sock]->sender_(_connects[sock]);
                }
            }
        }

    public:
        TcpServer(func_t func, uint16_t port = defaultport)
            : _service(func), _port(port), _revs(nullptr)
        {}

        // 初始化服务器
        void InitServer()
        {
            // 1. 创建socket
            _sock.Socket();    // 创建套接字
            _sock.Bind(_port); // 绑定端口号
            _sock.Listen();    // 监听

            // 2. 构建Epoll
            _epoller.Create();

            // 3. 将目前唯一的一个sock添加到epoller中
            // listensock也是一个套接字, 也要看作成一个connections
            AddConnection(_sock.Fd(), EPOLLIN | EPOLLET, 
                            bind(&TcpServer::Accepter, this, placeholders::_1), nullptr, nullptr);

            _revs = new struct epoll_event[num];
            _num = num;
        }

        //
        void EnableReadWrite(Connection *con, bool readable, bool writeable)
        {
            uint32_t event = (readable ? EPOLLIN : 0) | (writeable ? EPOLLOUT : 0) | EPOLLET;
            _epoller.Control(con->sock_, event, EPOLL_CTL_MOD);
        }

        // 事件派发器
        void Dispatch()
        {
            int timeout = -1;
            while (true)
            {
                Loop(timeout);
                //logMsg(DEBUG, "timeout ... ");
            }
        }

        ~TcpServer()
        {
            _sock.Close();
            _epoller.Close();
            if (_revs == nullptr) delete []_revs;
        }
    private:
        uint16_t _port;
        Sock _sock;
        Epoller _epoller;
        unordered_map<int, Connection*> _connects; // 管理所有的链接集合, key = 套接字
        struct epoll_event *_revs; // 事件的缓冲区
        int _num;
        //hander_t _handler;
        func_t _service;
    };
}

/*
我们对应的TCP服务器, 它一旦有了对应的底层epoll当中注册了事件,
未来一旦有事件就绪, 它就需要根据就绪的文件描述符在unordered_map里找到对应的Connection,
然后再调用connection曾经注册的对应的方法, 来进行我们称之为叫做: 事件派发
*/