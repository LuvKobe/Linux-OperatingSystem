#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <pthread.h>
#include <unistd.h>

using namespace std;

// 获取线程的tid, 并按十六进制打印
string changeId(const pthread_t &thread_id)
{
    char tid[128]; 
    snprintf(tid, sizeof(tid), "0x%x", thread_id);
    return tid;
}

// pthread_self()能获取线程ID
void* start_routine(void* args)
{
    sleep(1);
    string name = static_cast<char*>(args);
    //pthread_detach(pthread_self()); // 设置自己为分离状态

    int cnt = 5;
    while (cnt--)
    {
        cout << name << changeId(pthread_self()) <<endl;
        sleep(1);
    }
    return nullptr;
}

int main()
{
    // 创建线程
    pthread_t tid;
    pthread_create(&tid, nullptr, start_routine, (void*)"new thread 1 running... ");
    string main_id = changeId(pthread_self()); // 主线程的ID
    pthread_detach(tid); // 创建线程以后, 立马分离

    //cout << "main thread running, the id is "<< main_id << "...new thread id is " << changeId(tid) << endl;

    //sleep(2); // 主线程先sleep, 让新线程能够分离成功
    // 一个线程默认是joinable的, 如果设置了分离状态, 那么不能够进行等待了!
    //int n = pthread_join(tid, nullptr);
    //cout << "result: " << n << " --> " << strerror(n) << endl;

    while (true)
    {
        cout << "main thread running, the id is "<< main_id << "...new thread id is " << changeId(tid) << endl;
        sleep(1);
    }

    return 0;
}