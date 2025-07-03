#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <cerrno>
#include <cassert>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>


using namespace std;

// 把这个管道建立在/tmp目录下
#define NAMED_PIPE "/tmp/mypipe.88"

// 创建管道文件
bool createFifo(const string &path)
{
    umask(0);
    int n = mkfifo(path.c_str(), 0666);
    if (n == 0) // 创建管道文件成功
    {
        return true;
    }
    else // 创建管道文件失败
    {
        cout << "errno: " << errno << " err string: " << strerror(errno) << endl;
        return false; 
    }
}

// 删除管道文件
void removeFifo(const string &path)
{
    int n = unlink(path.c_str());
    assert(n == 0); // 移除成功为0
    (void)n;
}



