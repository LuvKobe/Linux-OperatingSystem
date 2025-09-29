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

class Thread;

// 上下文(当成一个大号的结构体来用)
class Context
{
public:
    // 构造函数
    Context() : _this(nullptr), _args(nullptr)
    {}

    // 析构函数
    ~Context()
    {}
public:
    Thread *_this;
    void *_args;
};

/*
assert(0 == n); 
(void)n;

为什么要写 (void)n 呢？

assert是在编译debug的方式发布的时候存在的,而release方式发布的时候, 就不存在了,
n是一个定义好的变量, 如果不使用的话, 在有些编译器下会出现warning提示,
所以加一句(void)n; 相当于是告诉编译器, 我使用了, 你别给我warning

另外: 意料之外用异常或者if判断; 意料之中用assert
*/

// 简单的一个线程封装
class Thread
{
public:
    /*
    function<void *(void *)>：
    表示一个可调用对象（函数、lambda、函数指针、仿函数等）；
    接收一个 void* 参数；
    返回一个 void*。
    */
    using func_t = function<void *(void *)>;
    const int num = 1024;
public:
    // 构造函数
    Thread(func_t func, void *args = nullptr, int number = 1) : _func(func), _args(args)
    {
        // C++写法
        //_name = "thread-";
        //_name += to_string(number);

        // 推荐用下面的C接口写法
        char buffer[num];
        snprintf(buffer, sizeof buffer, "thread-%d", number);
        _name = buffer;
    }

    // 在类内创建线程, 想让线程执行对应的方法, 我们需要将方法设置为staic, 因为static没有所谓的this指针
    static void* start_routine(void* args)
    {
        Context *ctx = static_cast<Context *>(args);
        void * ret = ctx->_this->run(ctx->_args);
        delete ctx;
        return ret;

        // 静态方法不能调用成员方法或者成员变量
    }

    // 线程启动
    void start()
    {
        //我们设置了一个context上下文, 它把当前的对象给我们传进来，把对应的给线程要执行的参数也给它设置进来。
        Context *ctx = new Context();
        ctx->_this = this;
        ctx->_args = _args;
        int n = pthread_create(&_tid, nullptr, start_routine, ctx); //TODO
        assert(0 == n); 
        (void)n;
    }

    // 线程等待
    void join()
    {
        int n = pthread_join(_tid, nullptr);
        assert(0 == n);
        (void)n;
        //cout << "result: " << n << " --> " << strerror(n) << endl;
    }

    void* run(void* args)
    {
        return _func(args);
    }

    // 析构函数
    ~Thread()
    {
        // do nothing...
    }
private:
    // 类的成员变量全部带_
    string _name;   // 线程名字
    func_t _func;   // 线程执行的函数
    void *_args;    // 线程的第四个参数

    pthread_t _tid; // 线程id
};

/*
那我给大家再解释一下吧，我们刚说了创建这个线程的时候，我们直接是不是执行我们对应的start_routine,
start_routine它直接传进来的不再是传说中的参数了，而是我们自定义的这个大号结构体Context,

而这个结构体呢, 在我们执行start()的时候就把它创建好。
创建好之后呢, 把它喂给线程执行的函数start_routine, 
而这个线程拿到这个对应的Context结构体的时候呢,
它把当前对象this, 和你要给这个线程调用函数值传递的参数_args都给我们设置好,
设置好了之后, 紧接着我们直接调用我们类内部的run()方法, 直接就可以把_func跑起来了.

*/