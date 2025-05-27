#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int global_val = 100;

int main()
{
    pid_t id = fork();
    
    if (id > 0)
    {
        // father
        printf("父进程pid: %d, ppid: %d, global_val: %d, &global_val: %p\n", getpid(), getppid(), global_val, &global_val);
    }
    else if (id == 0)
    {
        // child
        printf("子进程pid: %d, ppid: %d, global_val: %d, &global_val: %p\n", getpid(), getppid(), global_val, &global_val);
    }
    else 
    {
        perror("fork");
        return 1;
    }
    sleep(1);

    return 0;
}
