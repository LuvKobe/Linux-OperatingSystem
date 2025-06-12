#include <cstdio>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

int main()
{
    pid_t id = fork();
    assert(-1 != id);

    if (0 == id)
    {
        // child
        int cnt = 5;
        while (cnt)
        {
            printf("child running, pid is %d, ppid is %d\n", getpid(), getppid());
            cnt --;
            sleep(1);
        }
        exit(111);    
    }
    // parent
    // 1. 让OS释放子进程的Z状态
    // 2. 获取子进程的退出结果
    // 在等待期间, 子进程没有退出的时候, 父进程只能阻塞等待
    int status = 0; // child的退出信息
    int ret = waitpid(id, &status, 0);
    if (ret > 0) // 等待成功
    {
        //printf("wait successful, exit code: %d, signal code: %d\n", (status >> 8)&0xFF, status & 0x7F);
        
        // 判断是否正常退出
        if (WIFEXITED(status)) // 正常退出为真
        {
            // 判断子进程运行结果是否OK
            printf("exit code: %d\n", WEXITSTATUS(status));
        }
        else 
        {
            // 
            printf("child exit not normal!\n");
        }
    }

    return 0;
}
