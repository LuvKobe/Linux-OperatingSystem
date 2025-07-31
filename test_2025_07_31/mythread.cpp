#include <iostream>
#include <cassert>
#include <cstdio>
#include <string>
#include <pthread.h>
#include <unistd.h>

using namespace std;

int g_val = 0;

string func()
{
    return "我是一个独立的方法";
}

// 新线程执行函数
void* thread_routine(void *args)
{
    const char* name = (const char*)args;
    while (true)
    {
        //cout << "我是新线程, 我正在运行！我的名字是: " << name << endl;
        cout << "我是新线程, 我正在运行！我的名字是: " << name << " --> " << func() <<endl;
        sleep(1);
    }
}

int main()
{
    pthread_t tid;

    // 创建线程
    int n = pthread_create(&tid, nullptr, thread_routine, (void*)"thread one");
    assert(0 == n);
    (void)n;

    // 主线程
    while (true)
    {
        char tidbuffer[64];
        snprintf(tidbuffer, sizeof(tidbuffer), "0x%x", tid);
        //cout << "我是主线程, 我正在运行！我创建出来的线程的tid是: " << tidbuffer << endl;
        cout << "我是主线程, 我正在运行 --> " << func() << endl;
        sleep(1);
    }

    return 0;
}
