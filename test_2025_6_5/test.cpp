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
        
        extern char **environ;
        //char* const envp[] = { (char*)"MYENV=12345678", NULL };
        //execle("./mybin", "mybin", NULL, envp);
        putenv((char*)"MYENV=87654321"); // 将指定环境变量导入到系统中environ指向的环境变量表中
        execle("./mybin", "mybin", NULL, environ);

        exit(1); // exec一定出错了
    }
    // father
    int status = 0;
    pid_t ret = waitpid(id, &status, 0);
    if (ret > 0) // 等待成功
    {
        printf("wait successful, exit_code is %d, signal_code is %d\n", (status >> 8)&0xFF, status & 0x7F);
    }
}

