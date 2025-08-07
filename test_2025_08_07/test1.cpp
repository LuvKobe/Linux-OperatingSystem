#include "Thread.hpp"
#include <memory>

// 共享资源 --- 火车票
int tickets = 10000;

void *thread_run(void* args)
{
    //sleep(1);
    string work_type = static_cast<char*>(args);
    while (true)
    {
        cout << "I'm a new thread, and I'm doing: " << work_type << endl;
        sleep(1);
    }
}

int main()
{
    unique_ptr<Thread> thread1(new Thread(thread_run, (void*)"hello thread...", 1));
    unique_ptr<Thread> thread2(new Thread(thread_run, (void*)"print thread...", 2));
    unique_ptr<Thread> thread3(new Thread(thread_run, (void*)"log thread...", 3));
    unique_ptr<Thread> thread4(new Thread(thread_run, (void*)"log thread...", 4));

    
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
