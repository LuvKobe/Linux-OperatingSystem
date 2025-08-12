#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>

using namespace std;

int tickets = 100;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // 全局的互斥锁
pthread_cond_t cond = PTHREAD_COND_INITIALIZER; // 全局的条件变量

void* start_routine(void* args)
{
    string name = static_cast<const char*>(args);
    while (true)
    {
        pthread_mutex_lock(&mutex); // 加锁
        pthread_cond_wait(&cond, &mutex); // 所有的线程都在cond条件变量下等待
        cout << name << " -> " << tickets << endl;
        tickets--;
        pthread_mutex_unlock(&mutex); // 解锁
    }
}

int main()
{
    // 通过条件变量来控制线程的执行
    pthread_t t[5];
    for (int i = 0; i < 5; ++i)
    {
        char *name = new char[64];
        snprintf(name, 64, "thread %d", i + 1);
        pthread_create(t+i, nullptr, start_routine, name);
    }

    // 主线程来唤醒在该条件变量cond下等待的线程
    while (true)
    {
        sleep(1);
        //pthread_cond_signal(&cond); // 唤醒单个线程
        pthread_cond_broadcast(&cond); // 唤醒一批线程
        //cout << "main thread wakeup one thread..." << endl;
    }

    for (int i = 0; i < 5; ++i)
    {
        pthread_join(t[i], nullptr);
    }

    return 0;
}