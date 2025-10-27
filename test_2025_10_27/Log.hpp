#pragma once

#include <iostream>
#include <cstdio>
#include <string>
#include <cstdarg>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <unistd.h>

using namespace std;

#define DEBUG 0
#define NORMAL 1    // 正常
#define WARNING 2   // 警告
#define ERROR 3     // 错误,但是不影响代码运行
#define FATAL 4     // 致命错误

const char *to_levelstr(int level)
{
    switch (level)
    {
    case DEBUG: return "DEBUG";
    case NORMAL: return "NORMAL";
    case WARNING: return "WARNING";
    case ERROR: return "ERROR";
    case FATAL: return "FATAL";
    default: return nullptr;
    }
} 

// 封装一个日志函数
void logMsg(int level, const char *format, ...)  // 可变参数列表
{
    // [日志等级] [时间戳] [pid] [message]
    // [WARNING] [2024-08-28 14:30:05] [123] [创建socket失败]

    // 获取当前时间
    auto now = chrono::system_clock::now();
    time_t now_c = chrono::system_clock::to_time_t(now);
    struct tm local_time;

    // 使用 localtime_r（线程安全）
    localtime_r(&now_c, &local_time);

    // 手动格式化时间
    char time_str[20];
    snprintf(time_str, sizeof(time_str), "%04d-%02d-%02d %02d:%02d",
             1900 + local_time.tm_year, 
             1 + local_time.tm_mon, 
             local_time.tm_mday, 
             local_time.tm_hour, 
             local_time.tm_min);

#define NUM 1024
    
    char logprefix[NUM];
    snprintf(logprefix, sizeof(logprefix), "[%s] [%s] [pid: %d] ", to_levelstr(level), time_str, getpid());

    char logcontent[NUM];
    va_list arg;
    va_start(arg, format);

    vsnprintf(logcontent, sizeof(logcontent), format, arg);
    
    // 输出完整的日志
    cout << logprefix << logcontent << endl;
}