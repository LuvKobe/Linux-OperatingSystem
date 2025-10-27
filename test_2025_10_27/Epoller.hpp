#pragma once 

#include <iostream>
#include <sys/epoll.h>
#include "Log.hpp"
#include "Err.hpp"

using namespace std;

const static int defaultepfd = -1;
const static int size = 128;

// 对 epoll 做封装
class Epoller
{
public:
    Epoller()
        : _epfd(defaultepfd)
    {}

    ~Epoller()
    {
        if (_epfd != defaultepfd) close(_epfd);
    }
public:
    // 创建epoll
    void Create()
    {
        _epfd = epoll_create(size);
        if (_epfd < 0)
        {
            logMsg(FATAL, "epoll create error, code: %d, errstring: %s", errno, strerror(errno));
            exit(EPOLL_CREATE_ERR);
        }
    }

    // 添加listensock到epoll中
    // 用户 ---告诉---> 内核
    bool AddEvent(int sock, uint32_t events)
    {
        struct epoll_event ev;
        ev.events = events;
        ev.data.fd = sock;

        int n = epoll_ctl(_epfd, EPOLL_CTL_ADD, sock, &ev);
        return (n == 0);
    }

    // 内核 ---告诉---> 用户
    int Wait(struct epoll_event revs[], int num, int timeout)
    {
        int n = epoll_wait(_epfd, revs, num, timeout);
        return n; 
    }

    //
    bool Control(int sock, uint32_t event, int action)
    {
        int n = 0;
        if (action == EPOLL_CTL_MOD)
        {
            struct epoll_event ev;
            ev.events = event;
            ev.data.fd = sock;
            n = epoll_ctl(_epfd, action, sock, &ev);
        }
        else if (action == EPOLL_CTL_DEL)
        {
            n = epoll_ctl(_epfd, action, sock, nullptr);
        }
        else n = -1;

        return n == 0;
    }

    //
    void Close()
    {
        if (_epfd != defaultepfd)
            close(_epfd);
    }
private:
    int _epfd;
};