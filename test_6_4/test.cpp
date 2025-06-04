#include <cstdio>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

using namespace std;

int main()
{
    printf("process is running...\n");
    pid_t id = fork();
    assert(id != -1);

    if (0 == id)
    {
        sleep(1);
        // child
        // 类比: 命令行怎么写, 这里就怎么传
        
        //execl("/usr/bin/ls", "ls", "-a", "-l", "--color=auto", NULL);
        
        //execlp("ls", "ls", "-a", "-l", "--color=auto", NULL);
        
        //char *const argv[] = { "ls", "-a", "-l", "--color=auto", NULL };
        //execv("/usr/bin/ls", argv);
        
        //char *const argv[] = { "ls", "-a", "-l", "--color=auto", NULL };
        //execvp("ls", argv);

        execl("./mybin", "mybin", NULL);

        exit(1); // execl一定出错了
    }
    // father
    int status = 0;
    pid_t ret = waitpid(id, &status, 0);
    if (ret > 0) // 等待成功
    {
        printf("wait successful, exit_code is %d, signal_code is %d\n", (status >> 8)&0xFF, status & 0x7F);
    }
}

