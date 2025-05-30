#include <iostream>
#include <cstdio>
#include <unistd.h>

using namespace std;

int global_val = 100;

int main()
{
    pid_t id = fork();

    if (id > 0) 
    {
        // father 
        while (1)
        {
            printf("我是父进程, pid: %d, ppid: %d | global_val: %d, &global_val: %p\n", 
                    getpid(), getppid(), global_val, &global_val);
            sleep(2);
        }
    }
    else if (id == 0) 
    {
        // child 
        int cnt = 0;
        while (1)
        {
            printf("我是子进程, pid: %d, ppid: %d | global_val: %d, &global_val: %p\n", 
                    getpid(), getppid(), global_val, &global_val);
            sleep(1);
            cnt++;
            if (cnt == 10)
            {
                global_val = 300;
                printf("子进程已经更改了全局的变量啦.......\n");
            }
        }
    }
    else 
    {
        printf("fork error\n");
        return 1;
    }
    sleep(1);
    return 0;
}
