#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <pthread.h>
#include <unistd.h>

using namespace std;

// __thread可以将一个内置类型(int、char)设置为线程局部存储, 即给每个线程都来一份
__thread int g_val = 100;
//int g_val = 100;

// pthread_self()能获取线程ID
void* start_routine(void* args)
{
    sleep(1);
    string name = static_cast<char*>(args);

    int cnt = 5;   
    while (cnt--)
    {
        cout << name << "g_val: " << g_val << ", &g_val: " << &g_val << endl;
        sleep(1);
        g_val++; //新线程修改g_val的值
    }
    return nullptr;
}

int main()
{
    // 创建线程
    pthread_t tid;
    pthread_create(&tid, nullptr, start_routine, (void*)"new thread 1 running... ");
    pthread_detach(tid); // 创建线程以后, 立马分离

    while (true)
    {
        cout << "main thread running... " << "g_val: " << g_val << ", &g_val: " << &g_val << endl;
        sleep(1);
    }

    return 0;
}