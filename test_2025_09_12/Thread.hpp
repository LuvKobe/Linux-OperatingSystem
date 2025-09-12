// hpp文件可以把类的声明和定义放在一起
#pragma once

#include <iostream>
#include <functional>
#include <string>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <pthread.h>
#include <unistd.h>
using namespace std;

// 把Thread封装在命名空间里
namespace ThreadNS
{
    using func_t = function<void *(void *)>;
    const int num = 1024;

    // 简单的一个线程封装
    class Thread
    {
    private:
        // 在类内创建线程, 想让线程执行对应的方法, 我们需要将方法设置为staic, 因为static没有所谓的this指针
        static void *start_routine(void *args)
        {
            Thread *_this = static_cast<Thread *>(args);
            void *ret = _this->callback();
        }
    public:
        // 构造函数
        Thread()
        {
            // 推荐用下面的C接口写法
            char namebuffer[num];
            snprintf(namebuffer, sizeof namebuffer, "thread-%d", _threadnum++);
            _name = namebuffer;
        }

        // 线程启动
        void start(func_t func, void *args = nullptr)
        {
            _func = func;
            _args = args;
            int n = pthread_create(&_tid, nullptr, start_routine, this);
            assert(n == 0);
            (void)n;
        }

        // 线程等待
        void join()
        {
            int n = pthread_join(_tid, nullptr);
            assert(0 == n);
            (void)n;
        }

        // 析构函数
        ~Thread()
        {
            // do nothing...
        }
        
        // 获取线程名字
        string threadname()
        {
            return _name;
        }

        // 回调方法
        void *callback()
        {
            return _func(_args);
        }
    private:
        // 类的成员变量全部带_
        string _name; // 线程名字
        func_t _func; // 线程执行的函数
        void *_args;  // 线程的第四个参数

        pthread_t _tid; // 线程id

        static int _threadnum; // 线程编号
    };
    int Thread::_threadnum = 1;
}
