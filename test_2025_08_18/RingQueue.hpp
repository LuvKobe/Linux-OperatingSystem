#pragma once

#include <iostream>
#include <vector>
#include <cassert>
#include <semaphore.h>

using namespace std;

static const int gcap = 10;

template<class T>
class RingQueue
{
private:
    void P(sem_t &sem)
    {
        int n = sem_wait(&sem); // 等待信号量, 会将信号量的值减1
        assert(0 == n);
        (void)n;
    }

    void V(sem_t &sem)
    {
        int n = sem_post(&sem); // 发布信号量, 表示资源使用完毕, 可以归还资源了, 将信号量值加1。
        assert(0 == n);
        (void)n;
    }
    
public:
    RingQueue(const int cap = gcap) : _queue(cap), _cap(cap)
    {
        // 初始化信号量
        int n = sem_init(&_spaceSem, 0, _cap); // 生产者的信号值设置为_cap
        assert(0 == n);
        n = sem_init(&_dataSem, 0, 0); // 消费者的信号值设置为0
        assert(0 == n);
        (void)n;

        // 刚开始的时候, 生产者和消费者的下标都是0
        _productStep = _consumeStep = 0;
    }

    // 生产者接口: 向环形队列中投入数据
    void Push(const T& in)
    {
        // 1. 先申请信号量(申请到了空间信号量, 意味着我一定能进行正常的生产)
        P(_spaceSem); 
        
        // 2. 往下标放入数据
        _queue[_productStep] = in; 
        _productStep++;

        // 3. 防止下标越界, 保证是环形的递增
        _productStep %= _cap;

        // 4. 把数据资源给消费者
        V(_dataSem);
    }

    // 消费者接口: 从环形队列中拿出数据
    void Pop(T* out)
    {
        // 1. 先申请信号量(申请到了空间信号量, 意味着我一定能进行正常的消费)
        P(_dataSem);

        // 2. 开始消费(拿走数据)
        *out = _queue[_consumeStep];
        _consumeStep++;

        // 3. 防止下标越界, 保证是环形的递增
        _consumeStep %= _cap;

        // 4. 拿走数据以后, 该下标所指向的位置空出来了
        V(_spaceSem);
    }

    ~RingQueue()
    {
        // 销毁信号量
        sem_destroy(&_spaceSem);
        sem_destroy(&_dataSem);
    }
private:
    vector<T> _queue; // 用数组来模拟环形队列
    int _cap; // 队列容量
    sem_t _spaceSem; // 生产者想生产, 在队列中看中的是空间资源(也就是有没有空的位置)
    sem_t _dataSem; // 消费者想消费, 在队列中看中的是数据资源(也就是存放有没有数据)

    // 生产和消费的位置: 其实就是队列中的下标
    int _productStep;
    int _consumeStep; 
};