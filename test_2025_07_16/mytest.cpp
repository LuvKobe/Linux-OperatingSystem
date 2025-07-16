#include <iostream>
#include <unistd.h>
#include <sys/types.h>
using namespace std;

// 我写了一个将来一直会运行的程序, 用来进行后续的测试
int main()
{
    while (true)
    {
        cout << "我是一个进程, 我的pid是: " << getpid() << ", 我的ppid是: " << getppid() << endl;
        sleep(1);
    }

    return 0;
}