#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <pthread.h>
#include <unistd.h>
#include <memory>

#include "Mutex.hpp"

using namespace std;


// 定义一个全局锁
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 

// 共享资源 --- 火车票
int tickets = 1000;

void *getTicket(void* args)
{
    //sleep(1);
    string username = static_cast<char*>(args);
    while (true)
    {
        // 这个的 {} 是不想给 usleep(1000) 加锁
        {
            // RAII风格的加锁
            LockGuard lockguard(&lock); // 一条语句完成了加锁和解锁操作
            if (tickets > 0)
            {
                // 休眠的这段的时间模拟真实的抢票花费的时间
                usleep(1254); // 1s = 1000ms = 1000 000ws
                // 开始抢
                cout << username << " 正在进行抢票: " << tickets << endl;
                tickets--;
            }
            else
            {
                // 没票的话直接退出
                break;
            }
        }
        // 抢完票以后, 有一个形成订单的过程
        usleep(1000);
    }
    return nullptr;
}

int main()
{
    pthread_t t1, t2, t3, t4;
    pthread_create(&t1, nullptr, getTicket, (void*)"thread 1");
    pthread_create(&t2, nullptr, getTicket, (void*)"thread 2");
    pthread_create(&t3, nullptr, getTicket, (void*)"thread 3");
    pthread_create(&t4, nullptr, getTicket, (void*)"thread 4");

    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);
    pthread_join(t3, nullptr);
    pthread_join(t4, nullptr);

    return 0;
}

