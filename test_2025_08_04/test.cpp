#include <iostream>
#include <cassert>
#include <cstdio>
#include <pthread.h>
#include <unistd.h>

using namespace std;

// 该方法被主线程和新线程共享
string func()
{
    return "I am an independent method";
}

// 新线程执行函数
void* thread_routine(void *args)
{
    const char* name = (const char*)args;
    while (true)
    {
        cout << "I'am new thread, my name is " << name << " --> " << func() << endl;
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
    sleep(1);
    // 主线程
    while (true)
    {
        cout << "I'am main thread" << " --> " << func() << endl;
        sleep(1);
    }

    return 0;
}
