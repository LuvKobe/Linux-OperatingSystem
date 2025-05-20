#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

int main()
{
    pid_t id = fork();

    if (id == 0)
    {
        //child
        printf("我是子进程, pid: %d, ppid: %d\n", getpid(), getppid());
        sleep(5);
        exit(1); //让该进程直接终止
    }
    else 
    {
        //father
        while (1)
        {
            printf("我是父进程, pid: %d, ppid: %d\n", getpid(), getppid());
            sleep(1);
        }
    }
    return 0;
}
