#include "BlockQueue.hpp"
#include "Task.hpp"
#include <ctime>
#include <sys/types.h>
#include <unistd.h>

const string oper = "+-*/%";
int mymath(int x, int y, char op)
{
    int ret = 0;
    switch (op)
    {
    case '+':
        ret = x + y;
        break;
    case '-':
        ret = x - y;
        break;
    case '*':
        ret = x * y;
        break;
    case '/':
    {
        if (y == 0)
        {
            cerr << "div zero error!" << endl;
            ret = -1;
        }
        else
        {
            ret = x / y;
        }
    }
    break;
    case '%':
    {
        if (y == 0)
        {
            cerr << "mod zero error!" << endl;
            ret = -1;
        }
        else
        {
            ret = x % y;
        }
    }
    break;
    default:
        // do nothing
        break;
    }

    return ret;
}

// 消费者
void* consumer(void* _bq)
{
    BlockQueue<Task> *bq = static_cast< BlockQueue<Task>* >(_bq);
    while (true)
    {
        // 消费活动
        Task t;
        bq->pop(&t); // 获取对头数据
        cout << "Consume Task: " << t() << endl;
    }
    return nullptr;
}

// 生产者
void* producter(void* _bq)
{
    BlockQueue<Task> *bq = static_cast< BlockQueue<Task>* >(_bq);
    while (true)
    {
        // 生成活动
        int x = rand() % 10 + 1; // 在这里先用随机数构建一个数据
        int y = rand() % 5;
        int operCode = rand() % oper.size(); // 随机的操作符

        Task t(x, y, oper[operCode],mymath);
        bq->push(t); // 放入队列中
        cout << "Product Task: " << t.toTaskString() << endl;
        sleep(1);
    }
    return nullptr;
}

int main()
{
    srand((unsigned long)time(nullptr) ^ getpid());
    BlockQueue<Task> *bq = new BlockQueue<Task>(); // 构造一个阻塞队列的对象

    pthread_t c, p; // c:消费者 p:生产者
    // 让两个线程看到同一份阻塞队列
    pthread_create(&c, nullptr, consumer, bq);
    pthread_create(&p, nullptr, producter, bq);

    pthread_join(c, nullptr);
    pthread_join(p, nullptr);

    delete bq;
    return 0;
}