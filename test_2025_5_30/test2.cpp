#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

int addToTarget(int from, int to)
{
    int sum = 0;
    for (int i = from; i < to; i++)
      sum += i;
    //return sum;
    //_exit(31);
}

int main()
{
    printf("hello edison!");
    sleep(2);
    _exit(1);

    
    /*
    printf("hello edison\n");
    
    int ret = addToTarget(0, 100);
    printf("sum = %d\n", ret);

    while (1) 
        sleep(1);
    for (int i = 0; i < 10; i++)
    {
        printf("%d: %s\n", i, strerror(i));
    }
    int num = addToTarget(1, 100);
    if (num == 5050)
        return 0;
    else 
        return 1;
    */ 
}
