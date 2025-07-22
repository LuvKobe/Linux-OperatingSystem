#include <iostream>
#include <vector>
#include <signal.h>
#include <unistd.h>

using namespace std;

#define MAX_SIGNUM 31

static vector<int> sigarry = {2, 3};

static void show_pending(const sigset_t &pending)
{
    for (int signo = MAX_SIGNUM; signo >= 1; signo--)
    {
        // 判断当前的signo信号是否被pending
        if (sigismember(&pending, signo))
        {
            cout << "1";
        }
        else
            cout << "0";
    }
    cout << "\n";
}

int main()
{
    // 1. 先尝试屏蔽指定的信号
    sigset_t block, oblock, pending;

    // 1.1 初始化
    sigemptyset(&block);
    sigemptyset(&oblock);
    sigemptyset(&pending);

    // 1.2 添加要屏蔽的信号
    for (const auto &sig : sigarry)
    {
        sigaddset(&block, sig); // 屏蔽2号、3号信号
    }

    // 1.3 开始屏蔽, 设置进内核, 即进程的PCB中
    sigprocmask(SIG_SETMASK, &block, &oblock);

    // 2. 遍历打印所有的pending信号集
    while (true)
    {
        // 2.1 初始化
        sigemptyset(&pending);

        // 2.2 获取pending信号集
        sigpending(&pending);

        // 2.3 打印它
        show_pending(pending);

        // 3. 慢一点打印
        sleep(1);
    }

    return 0;
}