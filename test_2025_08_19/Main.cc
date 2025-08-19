#include "RingQueue.hpp"
#include "Task.hpp"
#include <pthread.h>
#include <cstdlib>
#include <ctime>
#include <sys/types.h>
#include <unistd.h>

// 单生产和单消费派发任务的过程

// 生产者
void* Producter(void* _rq)
{
    RingQueue<Task>* rq = static_cast< RingQueue<Task>* >(_rq);
    while (true)
    {        
        // 构建任务
        int x = rand() % 10;
        int y = rand() % 5;
        char op = oper[rand() % oper.size()];
        Task t(x, y, op, mymath);

        // 生产任务
        rq->Push(t);

        // 输出提示
        cout << "生产者派发任务: " << t.toTaskString() << endl;

        sleep(1);
    }
    return nullptr;
}

// 消费者
void* Consumer(void* _rq)
{
    sleep(1);
    RingQueue<Task>* rq = static_cast< RingQueue<Task>* >(_rq);
    while (true)
    {
        // 构建任务对象
        Task t;

        // 消费任务
        rq->Pop(&t);
        string result = t();
        cout << "消费者消费任务: " << result << endl;
        sleep(1);
    }
    return nullptr;
}

int main()
{   
    srand((unsigned int)time(nullptr) ^ getpid() ^ pthread_self() ^ 0x98794357); // 种一颗随机数种子
    
    RingQueue<Task>* rq = new RingQueue<Task>();
    pthread_t p, c;
    pthread_create(&p, nullptr, Producter, rq);
    pthread_create(&c, nullptr, Consumer, rq);

    pthread_join(p, nullptr);
    pthread_join(c, nullptr);
    delete rq;

    return 0;
}