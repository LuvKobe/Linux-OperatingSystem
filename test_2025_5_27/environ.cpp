#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

using namespace std;



// ./proc -a -b -c
int main()
{
    // 先声明一下
    extern char **environ;

    //打印环境变量
    for (int i = 0; environ[i]; i++)
    {
        printf("%d: %s\n", i, environ[i]);
    }


    // main函数不传这些参数的时候，也可以获取环境变量
    //printf("%s\n", getenv("PATH"));
        
    return 0;
}
