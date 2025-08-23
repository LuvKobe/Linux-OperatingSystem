#pragma once

// 封装一把锁

#include <iostream>
#include <pthread.h>

class Mutex
{
public:
    // 构造函数
    Mutex(pthread_mutex_t *lock_p = nullptr) : _lock_p(lock_p)
    {}

    // 加锁
    void lock()
    {
        if (_lock_p) // 如果锁不为空, 那么就加锁
        {
            pthread_mutex_lock(_lock_p);
        }
    }

    // 解锁
    void unlock()
    {
        if (_lock_p) // 如果锁不为空, 那么就解锁
        {
            pthread_mutex_unlock(_lock_p);
        }
    }

    // 析构函数
    ~Mutex()
    {}

private:
    pthread_mutex_t *_lock_p;
};

// 
class LockGuard
{
public:
    LockGuard(pthread_mutex_t* mutex) :_mutex(mutex)
    {
        _mutex.lock(); // 在构造函数中进行加锁
    }

    ~LockGuard()
    {
        _mutex.unlock(); // 在析构函数中进行解锁
    }
private:
    Mutex _mutex;
};