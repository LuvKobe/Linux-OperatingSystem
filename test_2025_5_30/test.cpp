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
        int cnt = 5;
        while (cnt)
        {
            printf("我是子进程: %d, 父进程: %d, cnt: %d\n", getpid(), getppid(), cnt--);
            sleep(1);
            
            //int a = 10;
            //a /= 0;
            
            int *p = NULL;
            *p = 100;
        }
        //exit(10); // 进程退出
    }
    // 父进程
    //sleet p(15);
    //pid_t ret = wait(NULL);
    int status = 0;
    pid_t ret = waitpid(id, &status, 0); // 设置为0表示阻塞时等待
    if (id > 0)
    {
        printf("wait success: %d, sig number: %d, child exit code: %d\n", ret, (status & 0x7F), (status >> 8) & 0xFF); 
    }
    sleep(5);

    return 0;
}
