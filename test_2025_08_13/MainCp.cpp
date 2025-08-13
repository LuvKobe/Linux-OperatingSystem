#include "BlockQueue.hpp"
#include <ctime>
#include <sys/types.h>
#include <unistd.h>

// 消费者
void* consumer(void* _bq)
{
    BlockQueue<int> *bq = static_cast< BlockQueue<int>* >(_bq);
    while (true)
    {
        // 消费活动
        int data;
        bq->pop(&data); // 获取对头数据
        cout << "Consume Data: " << data << endl;
    }
    return nullptr;
}

// 生产者
void* producter(void* _bq)
{
    //BlockQueue<int> *bq = static_cast< BlockQueue<int>* >(_bq);
    BlockQueue<int> *bq = static_cast< BlockQueue<int>* >(_bq);
    while (true)
    {
        // 生成活动
        int data = rand() % 10 + 1; // 在这里先用随机数构建一个数据
        bq->push(data); // 放入队列中
        cout << "Product Data: " << data << endl;
        sleep(1);
    }
    return nullptr;
}

int main()
{
    srand((unsigned long)time(nullptr) ^ getpid());
    BlockQueue<int> *bq = new BlockQueue<int>(); // 构造一个阻塞队列的对象

    pthread_t c, p; // c:消费者 p:生产者
    // 让两个线程看到同一份阻塞队列
    pthread_create(&c, nullptr, consumer, bq);
    pthread_create(&p, nullptr, producter, bq);

    pthread_join(c, nullptr);
    pthread_join(p, nullptr);

    delete bq;
    return 0;
}