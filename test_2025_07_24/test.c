#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h> 
#include <sys/types.h>
#include <sys/wait.h>



// volatile:保持内存的可见性
volatile int quit = 0;

static void myhandler(int signo)
{
    printf("%d号信号, 正在被捕捉!\n", signo);
    printf("quit: %d", quit);
    quit = 1;
    printf(" -> %d\n", quit);
}

int main()
{
    signal(2, myhandler);
    while (!quit);
    printf("注意, 我是正常退出的!\n");

    return 0;
}


/*
static void myhandler(int signo)
{
    // 正确的回收方式
    pid_t ret = waitpid(-1, NULL, WNOHANG);
    while (ret > 0)
    {
        printf("我的pid是%d, %d号信号正在被我捕捉!\n", getpid(), signo);
        printf("wait child success: %d\n", ret);
        sleep(1);
    }
    printf("子进程 %d 已经退出\n", getpid());
    printf("\n");
}

// 计数程序
void Count(int cnt)
{
    while (cnt)
    {
        printf("cnt: %2d\r", cnt);
        fflush(stdout);
        cnt--;
        sleep(1);
    }
    printf("\n");
}

int main()
{
    // 捕捉17号信号
    //signal(SIGCHLD, myhandler);
    signal(SIGCHLD, SIG_IGN);

    printf("我是父进程, pid: %d, ppid: %d\n", getpid(), getppid());

    pid_t id = fork();
    if (id == 0)
    {
        printf("我是子进程, pid: %d, ppid: %d, 我要退出啦！\n", getpid(), getppid());
        Count(5);
        exit(1);
    }
    while (1)
        sleep(1);
    return 0;
}*/