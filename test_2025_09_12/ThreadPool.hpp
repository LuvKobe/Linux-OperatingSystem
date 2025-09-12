#pragma once

/*
名称: 设计一套线程池:

目的: 一个用户他只需要向线程池当中去 push 任务, 或者是把自己的任务交给线程池, 然后他就不用管了，
    接着, 这个任务最终可以被指定的线程去获取并处理。
*/

#include "Thread.hpp"
#include "LockGuard.hpp"
#include "log.hpp"
#include <vector>
#include <queue>
#include <mutex>
#include <pthread.h>

using namespace ThreadNS;
using namespace std;

const int gnum = 10; // 初始化5个线程

// 声明
template <class T>
class ThreadPool;

// 定义线程数据结构体
template <class T>
class ThreadData
{
public:
    ThreadPool<T> *threadpool;
    string name;

public:
    ThreadData(ThreadPool<T> *tp, const string &n) : threadpool(tp), name(n) {}
};

template <class T>
class ThreadPool
{
private:
    // 线程执行的方法
    static void* handlerTask(void* args)
    {
        ThreadData<T>* td = (ThreadData<T> *)args;
        while (true)
        {
            T t;
            {
                // td->threadpool->lockQueue(); // 加锁
                LockGuard lockguard(td->threadpool->Mutex());    // 加锁
                while (td->threadpool->isQueueEmpty()) // 检测临界资源中是否有资源
                {
                    td->threadpool->threadWait(); // 如果没有, 就先休眠等待
                }
                t = td->threadpool->pop(); // pop的本质是: 将任务从公共队列中, 拿到当前线程自己独立的栈中
            }
            t();
        }
        delete td;
        return nullptr;
    }

    // 单例模式 --> 把构造函数设置成静态方法
    ThreadPool(const int& num = gnum) : _num(num)
    {   
        // 初始化: 锁、条件变量、线程
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_cond, nullptr);
        for (int i = 0; i < _num; ++i)
        {
            _threads.push_back(new ThreadNS::Thread()); // 把new出来的Thread对象尾插到vector中
        }
    }

    void operator=(const ThreadPool& ) = delete; // 删除赋值语句
    ThreadPool(const ThreadPool& ) = delete; // 删除拷贝构造

public: // 封装所有方法
    // 加锁方法
    void lockQueue() { pthread_mutex_lock(&_mutex); } 

    // 解锁方法
    void unlockQueue() { pthread_mutex_unlock(&_mutex); } 

    // 队列判空
    bool isQueueEmpty() { return _taskQueue.empty(); }

    // 线程等待
    void threadWait() { pthread_cond_wait(&_cond, &_mutex); }

    // 获取任务
    T pop() 
    { 
        T t = _taskQueue.front(); 
        _taskQueue.pop();
        return t;
    }

    // 获取锁的地址
    pthread_mutex_t *Mutex() { return &_mutex; }

public:
    // 线程启动
    void run()
    {
        for (const auto& t : _threads)
        {
            ThreadData<T>* td = new ThreadData<T>(this, t->threadname());
            t->start(handlerTask, td);
            //cout << t->threadname() << " start ..." << endl;
            logMsg(DEBUG, "%s start ...", t->threadname().c_str());
        }
    }

    // 往线程池中添加任务
    void Push(const T& in)
    {
        LockGuard lockguard(&_mutex);
        _taskQueue.push(in); // 往队列(临界资源)中添加任务
        pthread_cond_signal(&_cond); // 去唤醒在特定条件变量下等的线程
    }

    ~ThreadPool()
    {
        // 销毁: 锁、条件变量、线程
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_cond);
        for (const auto& t : _threads)
        {
            delete t;
        }
    }

    // 单例模式
    static ThreadPool<T> *getInstance()
    {
        if (nullptr == tp)
        {
            _siglock.lock(); // 加锁
            if (nullptr == tp)
            {
                tp = new ThreadPool<T>();
            }
            _siglock.unlock(); // 解锁
        }
        return tp;
    }

private:
    int _num; // 线程池中的线程个数
    vector<Thread*> _threads; // 用vector来管理这一批线程
    queue<T> _taskQueue; // 任务队列(临界资源)
    pthread_mutex_t _mutex; // 定义锁
    pthread_cond_t _cond; // 定义条件变量

    static ThreadPool<T>* tp; // 定义一个静态的指针
    static std::mutex _siglock; // 定义一个静态的锁(c++11的锁)
};

// 静态成员要在类外面初始化
template <class T>
ThreadPool<T>* ThreadPool<T>::tp = nullptr;

template <class T>
mutex ThreadPool<T>::_siglock;