#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <cstdio>

using namespace std;

// 构建一个任务, 放入阻塞队列中
class CalTask
{
    using func_t = function<int(int, int, char)>;
public:
    CalTask() {}

    CalTask(int x, int y, char op, func_t func) : _x(x), _y(y), _op(op), _callback(func)
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

const string oper = "+-*/%";
int mymath(int x, int y, char op)
{
    int ret = 0;
    switch (op)
    {
    case '+':
        ret = x + y;
        break;
    case '-':
        ret = x - y;
        break;
    case '*':
        ret = x * y;
        break;
    case '/':
    {
        if (y == 0)
        {
            cerr << "div zero error!" << endl;
            ret = -1;
        }
        else
        {
            ret = x / y;
        }
    }
    break;
    case '%':
    {
        if (y == 0)
        {
            cerr << "mod zero error!" << endl;
            ret = -1;
        }
        else
        {
            ret = x % y;
        }
    }
    break;
    default:
        // do nothing
        break;
    }

    return ret;
}

class SaveTask
{
    using func_t = function<void(const string&)>;
public:
    SaveTask() {}

    SaveTask(const string& msg, func_t func) : _msg(msg) , _func(func)
    {}

    void operator()()
    {
        _func(_msg);
    }
private:
    string _msg;
    func_t _func;
};

void Save(const string& msg)
{
    // 文件操作
    string target = "./log.txt";
    FILE* fp = fopen(target.c_str(), "a+");
    if (!fp)
    {
        cerr << "fopen error" << endl;
        return;
    }
    fputs(msg.c_str(), fp);
    fputs("\n", fp);

    fclose(fp);
}