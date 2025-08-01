#include <iostream>
#include <unistd.h>
#include <thread>

using namespace std;

//g++ -o myproc proc.cc -lpthread -std=c++11

// C++ 中使用线程库
void thread_run()
{
    while (true)
    {
        cout << "我是新线程..." << endl;
        sleep(1);
    }
}

int main()
{
    // 任何语音, 在Liunx中如果要实现多线程, 必定是要用thread库的
    // 如何看待C++11中的多线程呢？C++11的多线程，在Linux环境中，本质是对pthread库的封装
    thread t1(thread_run);

    // 主线程
    while (true)
    {
        cout << "我是主线程..." << endl;
        sleep(1);
    }

    t1.join();

    return 0;
}