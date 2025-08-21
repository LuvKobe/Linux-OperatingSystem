#include "ThreadPool.hpp"
#include "Task.hpp"
#include "unistd.h"
#include <memory>

/*
相当于我们写了一个小组件: 线程池, 它是生产者消费者模型。
你只需要把任务push到这个线程池里，那么线程池当中的线程会自动去进行处理。
因为我们有多个线程, 我自己手动的去给多线程派发任务, 那么最终每个线程没有任务的时候, 大家都在等; 有任务的时候，大家再处理.
*/

int main()
{
    // 获取单例
    ThreadPool<Task>::getInstance()->run();

    int x, y;
    char op;
    while (1)
    {
        cout << "请输入数据1# ";
        cin >> x;
        cout << "请输入数据2# ";
        cin >> y;
        cout << "请输入你要进行的运算# ";
        cin >> op;
        Task t(x, y, op, mymath);

        ThreadPool<Task>::getInstance()->Push(t);
        sleep(1);
    }

    return 0;
}














// // 设置一个方法
// void* run(void* args)
// {
//     string msg = (const char *)args;
//     while (true)
//     {
//         cout << msg << endl;
//         sleep(1);
//     }
//     return nullptr;
// }

// int main()
// {
//     ThreadNS::Thread t1(run, (void*)"thread 1 run");
//     ThreadNS::Thread t2(run, (void*)"thread 2 run");

//     t1.start();
//     t2.start();

//     t1.join();
//     t2.join();

//     return 0;
// }