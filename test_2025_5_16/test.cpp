#include <cstdio>
#include <iostream>
#include <unistd.h>

using namespace std;

int main()
{
    pid_t id = fork();
    

    // fork 之后代码共享
    if (id < 0) 
    {
        perror("fork");
        return 1;
    }
    else if (id == 0) 
    {
        //child
      while (1) 
      {
          printf("我是子进程, pid: %d, ppid: %d\n", getpid(), getppid());
          sleep(1);
      }
    }
    else {
        //parent
        while (2)
        {
            printf("我是父进程, pid: %d, ppid: %d\n", getpid(), getppid());
            sleep(3);
        }
    }

    return 0;
}
