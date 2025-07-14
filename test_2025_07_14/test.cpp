#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <signal.h>
#include <cassert>

// 给2号信号设置的默认捕捉方法
void handler(int signo)
{
    std::cout << "进程捕捉到了一个信号, 信号编号是: " << signo << std::endl;
}

int main()
{
    signal(2, handler); 
    while (true)
    {
        std::cout << "我是一个进程, 我的pid是: " << getpid() << ", 我的ppid是: " << getppid() << std::endl;
        sleep(1);
    }
    return 0;
}
