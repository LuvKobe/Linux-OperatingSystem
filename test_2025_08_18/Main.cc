#include "RingQueue.hpp"
#include <pthread.h>
#include <cstdlib>
#include <ctime>
#include <sys/types.h>
#include <unistd.h>

// 单生产和单消费

// 生产者
void* Producter(void* _rq)
{
    RingQueue<int>* rq = static_cast< RingQueue<int>* >(_rq);
    while (true)
    {        
        // 1. 先生产
        int data = rand() % 10 + 1;
        rq->Push(data);
        cout << "生产完成, 生产的数据是: " << data << endl;
    }
    return nullptr;
}

// 消费者
void* Consumer(void* _rq)
{
    RingQueue<int>* rq = static_cast< RingQueue<int>* >(_rq);
    while (true)
    {
        // 1. 先消费
        int data;
        rq->Pop(&data);
        cout << "消费完成, 消费的数据是: " << data << endl;
        sleep(1);
    }
    return nullptr;
}

int main()
{   
    srand((unsigned int)time(nullptr) ^ getpid() ^ pthread_self() ^ 0x98794357); // 种一颗随机数种子
    
    RingQueue<int>* rq = new RingQueue<int>();
    pthread_t p, c;
    pthread_create(&p, nullptr, Producter, rq);
    pthread_create(&c, nullptr, Consumer, rq);

    pthread_join(p, nullptr);
    pthread_join(c, nullptr);
    delete rq;

    return 0;
}