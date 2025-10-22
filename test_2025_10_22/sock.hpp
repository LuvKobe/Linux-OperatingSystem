#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "log.hpp"
#include "err.hpp"

// 封装一个套接字

class Sock
{
    const static int backlog = 32;

public:
    // 创建套接字
    static int Socket()
    {
        // 1. 创建socket文件套接字对象
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0)
        {
            logMsg(FATAL, "create socket error");
            exit(SOCKET_ERR);
        }
        logMsg(NORMAL, "create socket success: %d", sock);

        int opt = 1;
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

        return sock;
    }

    // 把套接字和对应的端口号绑定
    static void Bind(int sock, int port)
    {
        // 2. bind绑定自己的网络信息
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(port);
        local.sin_addr.s_addr = INADDR_ANY;
        if (bind(sock, (struct sockaddr *)&local, sizeof(local)) < 0)
        {
            logMsg(FATAL, "bind socket error");
            exit(BIND_ERR);
        }
        logMsg(NORMAL, "bind socket success");
    }

    // 设置套接字为监听状态
    static void Listen(int sock)
    {
        // 3. 设置socket 为监听状态
        if (listen(sock, backlog) < 0) // 第二个参数backlog后面在填这个坑
        {
            logMsg(FATAL, "listen socket error");
            exit(LISTEN_ERR);
        }
        logMsg(NORMAL, "listen socket success");
    }

    // 获取新链接
    static int Accept(int listensock, string *client_ip, uint16_t *client_port)
    {
        // 4. server 获取新链接
        // sock, 和 client进行通信的fd
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        int sock = accept(listensock, (struct sockaddr *)&peer, &len);
        if (sock < 0)
        {
            logMsg(ERROR, "accept error, next");
        }
        else
        {
            logMsg(NORMAL, "accept a new link success, get new sock: %d", sock); 
            *client_ip = inet_ntoa(peer.sin_addr);
            *client_port = ntohs(peer.sin_port);
        }
        // 如果失败则返回-1, 成功的话, 返回sock / ip / port
        return sock;
    }
};
