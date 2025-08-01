#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cstdio>
#include <string>
#include <vector>
#include <pthread.h>
#include <unistd.h>

using namespace std;

// 当成结构体使用
class ThreadData
{
public:
    int64_t number; //有符号整型
    pthread_t tid;
    char namebuffer[64];
};

class ThreadReturn
{
public:
    int exit_code;
    int exit_result;
};

// 新线程执行函数
// 1. 请问start_routine要被几个线程执行呢？当然是10个
//    请问start_routine函数现在是什么状态呢？被多个执行流访问叫做重入状态
// 2. 该函数是可重入函数吗？当然是的！
// 3. 在函数内定义的变量都叫做局部变量，具有临时性。那么在多线程情况下，也适用！
//    故每个线程都有自己独立的栈结构！线程之间内部定义的这些变量是不会互相影响！
void* start_routine(void *args)
{
    //sleep(1);

    ThreadData* td = static_cast<ThreadData*>(args); // 安全的进行强制类型转换
    int64_t cnt = 10;
    while (cnt)
    {
        cout << "cnt: " << (int64_t)cnt << " &cnt: " << &cnt << endl;
        cnt--;
        //cout << "new thread create success! my name is " << td->namebuffer << " cnt: " << cnt-- << endl;
        sleep(1);
    }

    // 线程如何终止呢？
    //delete td;
    //pthread_exit(nullptr);

    //return nullptr; // 线程函数结束, return的时候, 线程就算终止了
    return (void*)2025; // 做右值
    // ThreadReturn *tr = new ThreadReturn();
    // tr->exit_code = 1;
    // tr->exit_result = 2025; 

    // return (void*)tr;
}


int main()
{   
    // 1. 创建一批线程(10个)
    vector<ThreadData*> threads;
#define NUM 10
    for (int i = 0; i < NUM; ++ i)
    {
        ThreadData *td = new ThreadData(); // 主线程在执行
        snprintf(td->namebuffer, sizeof(td->namebuffer), "%s:%d", "thread", i + 1); // 主线程在执行
        pthread_create(&td->tid, nullptr, start_routine, td); // 创建新线程
        
        // 主线程拿到10个新线程的数据
        threads.push_back(td); 
    }

    // 主线程打印出来10个新线程的信息
    for (auto &iter : threads)
    {
        cout << "create thread: " << iter->namebuffer << " : " << iter->tid << " success" << endl;
    }
    //线程是可以被取消的，但是线程要被取消，前提是这个线程已经跑起来了
    //线程如果是被取消的, 那么退出码是 -1
    sleep(5);
    for (int i = 0; i < threads.size() / 2; ++ i)
    {
        int n = pthread_cancel(threads[i]->tid);
        assert(n == 0);
        cout << "pthread_cancel : " << threads[i]->namebuffer << " success" << endl;
    }

    // 线程的等待
    for (auto &iter : threads)
    {
        void *ret = nullptr;
        int n = pthread_join(iter->tid, (void**)&ret);
        assert(n == 0);
        cout << "join : " << iter->namebuffer << " success, exit_code: " << (int64_t)ret << endl;
        //cout << "join : " << iter->namebuffer << " success, exit_code: " << ret->exit_code << ", exit_result: " << ret->exit_result << endl;
        delete iter;
    }

    cout << "main thread quit!" << endl;
    return 0;
}
