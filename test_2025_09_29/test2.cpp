#include "Thread.hpp"
#include <memory>

/*
如果是一个局部的锁, 那么必须要用 pthread_mutex_init 和 pthread_mutex_destroy 进行初始化和销毁;
如果是一个全局或静态的锁, 我们直接调用, 只需要用 pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER 进行初始化即可, 不需要销毁。
*/

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // 定义一个全局锁

// 共享资源 --- 火车票
int tickets = 10000;

/*
1. 多个执行流进行安全访问的共享资源叫做：临界资源。
   在加锁和解锁之间的这份代码就是传说中的临界资源

2. 我们把多个执行流中, 访问临界资源的代码叫做：临界区, 它往往是线程代码很小的一部分
   那么其中我们把访问临界资源的代码，称之为临界区

3. 我们想让多个线程串行的访问共享资源，叫做：互斥

4. 我们对一个资源进行访问的时候, 要么不做, 要么做完, 这种叫做：原子性
   即一个对资源进行的操作, 如果只用一条汇编就能完成, 那么就是原子性;
   反之, 则不是原子性。

5. 故提出解决方案为: 加锁！
*/

/*
尽可能的让多个线程交叉执行,
多个线程交叉执行的本质：就是让调度器尽可能的频繁发生线程调度与切换,
线程一般在什么时候发生切换呢？时间片到了, 来了更高优先级的线程,
线程是在什么检测到上面的问题呢？从内核态返回用户态的时候, 线程要对调度状态进行检测, 如果可以, 就直接发生线程切换.
*/

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
    }
    return nullptr;
}

int main()
{
    /*
    std::unique_ptr 是 C++11 引入的智能指针类型，用来 独占式地管理动态内存。
    它能自动释放资源，避免内存泄漏，是裸指针 new/delete 的现代替代品。
    */
    unique_ptr<Thread> thread1(new Thread(getTicket, (void*)"user 1", 1));
    unique_ptr<Thread> thread2(new Thread(getTicket, (void*)"user 2", 2));
    unique_ptr<Thread> thread3(new Thread(getTicket, (void*)"user 3", 3));
    unique_ptr<Thread> thread4(new Thread(getTicket, (void*)"user 4", 4));

    thread1->start();
    thread2->start();
    thread3->start();
    thread4->start();

    thread1->join();
    thread2->join();
    thread3->join();
    thread4->join();

    return 0;
}

