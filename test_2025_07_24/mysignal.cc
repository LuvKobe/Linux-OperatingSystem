#include <iostream>
#include <stdio.h>
#include <vector>
#include <signal.h>
#include <unistd.h>

using namespace std;

// 计数程序
// void Count(int cnt)
// {
//     while (cnt)
//     {
//         printf("cnt: %2d\r", cnt);
//         fflush(stdout);
//         cnt--;
//         sleep(1);
//     }
//     printf("\n");
// }

void myhandler(int signo)
{
    cout << "get a signo: " << signo << " 号信号正在处理中..." << endl;
    //Count(20);
}

int main()
{
    // 自定义捕捉2号信号
    struct sigaction act, oact;
    act.sa_handler = myhandler;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask); // 当我们正在处理某一种信号的时候, 我们也想顺便屏蔽其他信号, 就可以添加到这个sa_mask中
    //sigaddset(&act.sa_mask, 3); // 比如处理2号信号的同时, 屏蔽3号信号
    sigaction(SIGINT, &act, &oact);

    while (true)
        sleep(1);

    return 0;
}

struct Node
{
    int x;
    struct Node *next;
};

struct Node *head = NULL;