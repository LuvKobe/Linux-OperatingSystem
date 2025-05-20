#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

int main()
{
    while (1) 
    {
        printf("pid: %d\n", getpid());
        sleep(1);
    }
    return 0;
}
