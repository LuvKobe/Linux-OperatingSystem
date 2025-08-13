#pragma once

#include <iostream>
#include <queue>
#include <pthread.h>

using namespace std;

template<class T>
class BlockQueue
{
public:
    static const int gmaxcap;
public:
    BlockQueue(const int& maxcap = gmaxcap):_maxcap(maxcap)
    {
        pthread_mutex_init(&_mutex, nullptr); // 初始化锁
        pthread_cond_init(&_pcond, nullptr);
        pthread_cond_init(&_ccond, nullptr);
    }

    // 往队列中放数据 (生产者调用)
    void push(const T &in) // 输入型参数: const &
    {
        pthread_mutex_lock(&_mutex);

        // 1. 判断
        // 细节2: 充当条件判断的语法必须是while, 不能用if
        while (is_full()) // 如果队列满了, 那么就不能生产了
        {
            // 细节1: pthread_cond_wait 这个函数的第二个参数, 必须是我们正在使用的互斥锁
            //    a. pthread_cond_wait: 该函数调用的时候, 会以原子性的方式, 将锁释放, 并将自己挂起
            //    b. pthread_cond_wait: 该函数在被唤醒返回的时候, 会自动的重新获取你传入的锁
            pthread_cond_wait(&_pcond, &_mutex); // 此时生产者进行阻塞等待
        }

        // 2. 走到这里我们能保证队列没满, 那么就push
        _q.push(in);

        // 3. 此时, 绝对能保证阻塞队列里面有数据, 那么就唤醒消费者
        // 细节3: pthread_cond_signal这个函数可以放在临界区内部, 也可以放在外部
        pthread_cond_signal(&_ccond);

        pthread_mutex_unlock(&_mutex);
    }

    // 从队列中拿出数据 (消费者调用)
    void pop(T *out) // 输出型参数: *   
    {
        pthread_mutex_lock(&_mutex);

        // 1. 判断
        while (is_empty()) // 如果队列空了, 那么就不能消费了
        {
            pthread_cond_wait(&_ccond, &_mutex); // 此时消费者进行阻塞等待
        }

        // 2. 走到这里我们能保证, 队列一定不为空
        *out = _q.front(); // 拿出队头数据
        _q.pop();

        // 3. 当我们在第2步拿走一个数据以后, 走到这里我们能保证, 阻塞队列里面至少有一个空的位置
        pthread_cond_signal(&_pcond); // 既然有一个空的位置, 那么就唤醒生产者

        pthread_mutex_unlock(&_mutex);
    }

    ~BlockQueue()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_pcond);
        pthread_cond_destroy(&_ccond);
    }
private:
    bool is_empty()
    {
        return _q.empty();
    }
    bool is_full()
    {
        return _q.size() == _maxcap;
    }
private:
    queue<T> _q; // 队列
    int _maxcap; // 队列的最大容量
    pthread_mutex_t _mutex; // 定义一把锁, 保证临界资源的安全性
    pthread_cond_t _pcond; // 生产者对应的条件变量(如果队列满了, 那么生产者就不能生产了)
    pthread_cond_t _ccond; // 消费者对应的条件变量(如果队列空了, 那么消费者就不能消费了)
};

//提供类外定义
template<class T>
const int BlockQueue<T>::gmaxcap = 5;