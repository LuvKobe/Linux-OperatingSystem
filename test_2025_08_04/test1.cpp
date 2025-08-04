#include <iostream>
#include <cstdio>
#include <vector>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

#define CHILD_NUM 2

// 信号处理函数
static void myhandler(int signo)
{
    pid_t ret;
    while ((ret = waitpid(-1, NULL, WNOHANG)) > 0)
    {
        printf("父进程 %d 捕捉到 %d 号信号，成功回收子进程 %d\n", getpid(), signo, ret);
        sleep(1);
    }
}

// 每个子进程执行不同任务（现在只是一条简单输出）
void do_task(int task_id)
{
    printf("子进程 %d 执行任务：我是编号为 %d 的任务\n", getpid(), task_id);
    printf("\n");
}

int main()
{
    // 注册信号处理
    signal(SIGCHLD, myhandler);

    printf("我是父进程, pid: %d\n", getpid());
    cout << endl;

    for (int i = 0; i < CHILD_NUM; ++i)
    {
        pid_t id = fork();
        if (id == 0)
        {
            // 子进程
            printf("子进程 %d 开始执行任务，ppid: %d\n", getpid(), getppid());
            do_task(i);
            sleep(3);
            exit(1);  // 子进程正常退出
        }
    }

    // 父进程等待子进程退出
    while (1)
        sleep(1);

    return 0;
}