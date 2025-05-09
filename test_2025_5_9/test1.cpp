#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>

using namespace std;

int func1(int a, int b)
{
    return a + b;
}

int func2(int a, int b)
{
    return a * b;
}

int main()
{
    int a = 10, b = 20;
    pid_t id = fork();
    if (id == 0)
    {
        // 子进程
        int ret = func1(a, b);
        printf("a + b = %d\n", ret);
    }
    else if (id > 0)
    {
        // 父进程
        int ret = func2(a, b);
        printf("a * b = %d\n", ret);
    }
    else 
    {
        perror("fork");
    }

    return 0;
}
