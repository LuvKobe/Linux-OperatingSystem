#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <cassert>
#include <string>
#include <sys/types.h>

using namespace std;

// 给2号信号设置的默认捕捉方法
// void handler(int signo)
// {
//     std::cout << "进程捕捉到了一个信号, 信号编号是: " << signo << std::endl;
// }

// int main()
// {
//     signal(2, handler); 
//     while (true)
//     {
//         std::cout << "我是一个进程, 我的pid是: " << getpid() << ", 我的ppid是: " << getppid() << std::endl;
//         sleep(1);
//     }
//     return 0;
// }


static void Usage(const string &proc)
{
    cout << "\nUsage: " << proc << " pid signo\n" << endl;
}

// int main(int argc, char *argv[])
// {
//     // 系统调用向目标进程发送信号
//     // kill() 可以向任意进程发送任意信号
//     if (argc != 3)
//     {
//         Usage(argv[0]);
//         exit(1);
//     }
//     pid_t pid = atoi(argv[1]);
//     int signo = atoi(argv[2]);
//     int n = kill(pid, signo);
//     if (n != 0)
//     {
//         perror("kill");
//     }

//     return 0;
// }


// int main(int argc, char *argv[])
// {
//     // 系统调用向目标进程发送信号
//     // raise 给自己发送信号
//     
//     int cnt = 0;
//     while (cnt <= 10)
//     {
//         printf("cnt: %d\n", cnt++);
//         sleep(1);
//         if (cnt >= 5)
//         {
//             raise(3); // 当cnt=5时, 给自己发送3号信号
//         }
//     }

//     return 0;
// }

// int main(int argc, char *argv[])
// {
//     // 系统调用向目标进程发送信号
//     // abort 给自己发送 指定的信号SIGABRT == kill(getpid, SIGABRT)
//     int cnt = 0;
//     while (cnt <= 10)
//     {
//         printf("cnt: %d, pid: %d\n", cnt++, getpid());
//         sleep(1);
//         if (cnt >= 5)
//         {
//             abort(); // 当cnt=5时, 给自己发送3号信号
//         }
//     }

//     return 0;
// }

// // 自定义捕捉信号
// void catchSig(int signo)
// {
//     cout << "获取到一个信号, 信号编号是: " << signo << endl;
//     exit(0);
// }

// int main()
// {
//     signal(SIGSEGV, catchSig); 

//     while (true)
//     {
//         cout << "我在运行中......" << endl;
//         sleep(1);
//         int *p = nullptr;
//         *p = 100;
//     }
//     return 0;
// }

// int cnt = 0;

// // 自定义捕捉信号
// void catchSig(int signo)
// {
//     cout << "获取到一个信号, 信号编号是: " << signo << ", cnt = " << cnt << endl;
//     //exit(0);
//     alarm(1);
// }

// int main()
// {
//     // 统计1s左右, 计算机能够将数据累计相加多生成!
//     signal(SIGALRM, catchSig);
//     alarm(1); 

//     while (true)
//     {
//         cnt++;
//     }
//     return 0;
// }



int cnt = 0;

// 自定义捕捉信号
void catchSig(int signo)
{
    cout << "获取到一个信号, 信号编号是: " << signo << ", cnt = " << cnt << endl;
    //exit(0);
}

int main()
{
    // 核心转储
    while (true)
    {
        int a[10];
        a[10000] = 106; // 数组越界
    }

    return 0;
}