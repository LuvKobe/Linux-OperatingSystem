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
#include <memory>

// 定义一个全局锁
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 

// 共享资源 --- 火车票
int tickets = 10000;

void *getTicket(void* args)
{
    //sleep(1);
    string username = static_cast<char*>(args);
    while (true)
    {
        pthread_mutex_lock(&lock); // 对临界区进行加锁
        if (tickets > 0)
        {
            // 休眠的这段的时间模拟真实的抢票花费的时间
            usleep(1254); // 1s = 1000ms = 1000 000ws
            // 开始抢
            cout << username << " 正在进行抢票: " << tickets << endl;
            tickets--;
            pthread_mutex_unlock(&lock); // 对临界区进行解锁
        }
        else
        {
            // 没票的话直接退出
            pthread_mutex_unlock(&lock); // 对临界区进行加解锁
            break;
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

