#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
using namespace std;


int main()
{
    pid_t id = fork();
    
    if (id == 0)
    {
        // child
        int cnt = 10;
        while (cnt)
        {
            printf("我是子进程: %d, 父进程: %d, cnt: %d\n", getpid(), getppid(), cnt--);
            sleep(1);
        }
        exit(0); // 进程退出
    }
    // 父进程
    sleep(15);
    pid_t ret = wait(NULL);
    if (id > 0)
    {
        printf("wait success: %d\n", ret);
    }
    sleep(5);

    return 0;
}
