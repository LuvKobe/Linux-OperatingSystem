#pragma once

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <cerrno>
#include <cstdio>

using namespace std;

// 把文件描述符设置为非阻塞
class Util
{
public:
    // 设置非阻塞的函数
    static bool SetNonBlock(int fd)
    {
        int fl = fcntl(fd, F_GETFL);
        if (fl < 0)
        {
            //cerr << "fcntl : " << strerror(errno) << endl;
            return false;
        }
        fcntl(fd, F_SETFL, fl | O_NONBLOCK);
        return true;
    }
};