#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <unistd.h>
using namespace std;

int main()
{
    // 创建子进程
    pid_t id = fork();
    
    if (id == 0) 
    {
        // 子进程
        while (1)
        {
            printf("我是子进程, 我的pid是: %d, 我的ppid是: %d, 我的id是: %d\n", getpid(), getppid(), id);
            sleep(1);
        }
    }
    else if (id > 0)
    {   
        // 父进程
        while (1)
        {
            printf("我是父进程, 我的pid是: %d, 我的ppid是: %d, 我的id是: %d\n", getpid(), getppid(), id);
            sleep(2);
        }
    }
    else {
        perror("fork");
        return 1;
    }

    sleep(2);
    
    return 0;
}
