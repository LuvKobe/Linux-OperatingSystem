#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <cstdio>
#include <unistd.h>
#include "log.hpp"  

using namespace std;

// 进行通信的读写(第一版)
void serviceIO(int sock)
{
    char buffer[1024];
    while (true)
    {
        ssize_t n = read(sock, buffer, sizeof(buffer) - 1);
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
    close(sock);
}

// 构建一个任务, 放入阻塞队列中
class Task
{
    using func_t = function<void(int)>;

public:
    Task() {}

    Task(int sock, func_t func) 
        : _sock(sock), _callback(func)
    {}

    void operator()()
    {
        _callback(_sock);
    }

private:
    int _sock;
    func_t _callback; // 回调方法
};