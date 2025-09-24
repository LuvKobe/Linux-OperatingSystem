#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <pthread.h>
#include <unistd.h>

using namespace std;

class ThreadData
{
public:
    ThreadData(const string &threadname, pthread_mutex_t *mutex_p) : _threadname(threadname), _mutex_p(mutex_p)
    {}

    ~ThreadData() {}

public:
    string _threadname;
    pthread_mutex_t *_mutex_p;
};

// 共享资源 --- 火车票
int tickets = 10000;

void *getTicket(void* args)
{
    //sleep(1);
    ThreadData *td = static_cast<ThreadData*>(args);
    while (true)
    {
        // 加锁和解锁的过程是多个线程串行执行的, 所以程序变慢了
        // 锁只规定互斥访问, 没有规定必须让谁优先执行
        // 锁就是真正的让多个执行流进行竞争的结果
        pthread_mutex_lock(td->_mutex_p); // 对临界区进行加锁
        if (tickets > 0)
        {
            // 休眠的这段的时间模拟真实的抢票花费的时间
            usleep(1254); // 1s = 1000ms = 1000 000ws
            // 开始抢
            cout << td->_threadname << " 正在进行抢票: " << tickets << endl;
            tickets--;
            pthread_mutex_unlock(td->_mutex_p); // 对临界区进行解锁
        }
        else
        {
            // 没票的话直接退出
            pthread_mutex_unlock(td->_mutex_p); // 对临界区进行加解锁
            break;
        }
        // 抢完票以后, 有一个形成订单的过程
        usleep(1000);
    }
    return nullptr;
}

int main()
{
#define NUM 4

    pthread_mutex_t lock;
    pthread_mutex_init(&lock, nullptr); // 初始化锁
    vector<pthread_t> tids(NUM);

    // 创建线程
    for (int i = 0; i < NUM; ++ i)
    {
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "thread %d", i+1);
        ThreadData* td = new ThreadData(buffer, &lock);
        pthread_create(&tids[i], nullptr, getTicket, td);
    }

    // 等待线程
    for (const auto &tid : tids)
    {
        pthread_join(tid, nullptr);
    }

    pthread_mutex_destroy(&lock); // 销毁锁

    return 0;
}