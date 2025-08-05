#include <iostream>
#include <string>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <pthread.h>
#include <unistd.h>

using namespace std;

// // 该方法被主线程和新线程共享
// string func()
// {
//     return "I am an independent method";
// }

// 新线程执行函数
// void* thread_routine(void *args)
// {
//     string name = static_cast<const char*>(args); // 安全的进行强制类型转换
//     while (true)
//     {
//         cout << "new thread create success! my name is " << name << endl;
//         sleep(1);
//     }
//     return nullptr;
// }

// int main()
// {
//     pthread_t tid;

//     // 创建线程
//     int n = pthread_create(&tid, nullptr, thread_routine, (void*)"thread one");
//     assert(0 == n);
//     (void)n;
//     sleep(1);

//     // 主线程延时3秒后请求取消子线程
//     sleep(3);
//     int m = pthread_cancel(tid);  // 请求取消子线程
//     assert(m == 0);
//     cout << "pthread_cancel : " << tid << " success" << endl;

//     // 主线程
//     // while (true)
//     // {
//     //     cout << "I'am main thread" << endl;
//     //     sleep(1);
//     // }

//     return 0;
// }

// // 方法 1：通过 return 返回值退出
// void *thread1(void *arg)
// {
//     printf("thread 1 returning ... \n");
//     int *p = (int *)malloc(sizeof(int));
//     *p = 1;
//     return (void *)p; //线程退出时 return 一个值（返回堆上分配的地址）。
// }

// // 方法 2：通过 pthread_exit 返回值退出
// void *thread2(void *arg)
// {
//     printf("thread 2 exiting ...\n");
//     int *p = (int *)malloc(sizeof(int));
//     *p = 2;
//     pthread_exit((void *)p); //与 return 效果等价，只是使用 pthread_exit() 明确表达线程退出。
// }

// void *thread3(void *arg)
// {
//     while (1)
//     { 
//         printf("thread 3 is running ...\n");
//         sleep(1);
//     }
//     return NULL; // 实际不会走到这里
// }

// int main()
// {
//     pthread_t tid;
//     void *ret = nullptr;

//     // // thread 1 return
//     // pthread_create(&tid, NULL, thread1, NULL);
//     // pthread_join(tid, &ret);
//     // printf("thread return, thread id %X, return code: %d\n", tid, *(int *)ret);
//     // free(ret);

//     // // thread 2 exit
//     // pthread_create(&tid, NULL, thread2, NULL);
//     // pthread_join(tid, &ret);
//     // printf("thread return, thread id %X, return code: %d\n", tid, *(int *)ret);
//     // free(ret);

//     // thread 3 cancel by other
//     pthread_create(&tid, NULL, thread3, NULL);
//     sleep(3);
//     pthread_cancel(tid);
//     pthread_join(tid, &ret);
//     if (ret == PTHREAD_CANCELED) 
//     {
//         printf("thread return, thread id %X, return code: PTHREAD_CANCELED\n", tid);
//     }
//     else 
//     {
//         printf("thread return, thread id %X, return code: NULL\n", tid);
//     }

//     return 0;
// }



// void* thread_func(void* arg)
// {
//     printf("Child thread started (TID: %lu)\n", pthread_self());
//     sleep(2);
//     printf("Child thread finished\n");
//     return nullptr;
// }

// int main()
// {
//     pthread_t tid;
    
//     // 创建线程
//     if (pthread_create(&tid, NULL, thread_func, NULL) != 0) 
//     {
//         perror("Failed to create thread");
//         exit(1);
//     }

//     // 分离子线程（主线程不再关心它的返回值）
//     if (pthread_detach(tid) != 0) 
//     {
//         perror("Failed to detach thread");
//         exit(1);
//     }

//     printf("Main thread: detached child thread, no join needed.\n");

//     // 等待足够时间让子线程完成
//     sleep(3);
//     printf("Main thread exiting.\n");

//     return 0;
// }

// void *thread_func(void *arg)
// {
//     // 自我分离
//     pthread_detach(pthread_self());

//     printf("Detached thread running...\n");
//     sleep(2);
//     printf("Detached thread exiting...\n");

//     return nullptr;
// }

// int main()
// {
//     pthread_t tid;
//     int n = pthread_create(&tid, nullptr, thread_func, nullptr);
//     assert(0 == n);
//     (void)n;

//     // 不用 pthread_join(tid)，也不会内存泄漏
//     sleep(3); // 等待子线程退出
//     printf("Main thread exiting.\n");
    
//     return 0;
// }


void *thread_func(void *args)
{
    const char* name = static_cast<const char*>(args); // 安全的进行强制类型转换

    // 自我分离
    pthread_detach(pthread_self());
    printf("Detached %s...\n", name);

    return nullptr;
}

int main()
{
    pthread_t tid;
    int n = pthread_create(&tid, nullptr, thread_func, (void*)"thread 1 run...");
    assert(0 == n);
    (void)n;

    // 让子线程在主线程 join() 前有时间自我 detach，否则竞争条件下可能主线程先 join()，就不会失败。
    sleep(1); //很重要，要让线程先分离，再等待

    
    //如果此时主线程调用 pthread_join(tid)，会失败，返回 EINVAL。
    int join_ret = pthread_join(tid, NULL);
    if (join_ret == 0)
    {
        printf("pthread wait success\n");
    }
    else
    {
        printf("pthread wait failed, return = %d (%s)\n", join_ret, strerror(join_ret));
    }

    return 0;
}