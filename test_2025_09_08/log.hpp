#pragma once

#include <iostream>
#include <string>

using namespace std;

#define DEBUG 0
#define NORMAL 1    // 正常
#define WARNING 2   // 警告
#define ERROR 3     // 错误,但是不影响代码运行
#define FATAL 4     // 致命错误

// 封装一个日志函数
void logMsg(int level, const string &msg)
{
    // [日志等级] [时间戳] [pid] [message]
    // [WARNING] [2024-08-28 14:30:05] [123] [创建socket失败]

    // 暂定
    cout << msg << endl;
}