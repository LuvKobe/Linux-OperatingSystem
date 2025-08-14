#pragma once

#include <iostream>
#include <functional>
#include <cstdio>

using namespace std;

// 构建一个任务, 放入阻塞队列中
class Task
{
    using func_t = function<int(int, int, char)>;
public:
    Task() {}

    Task(int x, int y, char op, func_t func) : _x(x), _y(y), _op(op), _callback(func)
    {}

    string operator()()
    {
        int ret = _callback(_x, _y, _op);

        char buffer[1024];
        snprintf(buffer, sizeof buffer, "%d %c %d = %d", _x, _op, _y, ret);
        return buffer;
    }

    string toTaskString()
    {
        char buffer[1024];
        snprintf(buffer, sizeof buffer, "%d %c %d = ?", _x, _op, _y);
        return buffer;
    }
private:
    int _x;
    int _y;
    char _op;
    func_t _callback; // 回调方法
};