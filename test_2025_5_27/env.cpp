#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <string.h>

using namespace std;

// ls -a -b -c -d -e ---> 长字符串
// "ls" "-a" "-b" "-c" "-d" "-e"


// ./proc -a -b -c
int main(int argc, char* argv[], char *env[])
{     
    
    for (int i = 0; env[i]; i++)
    {
        printf("env[%d]: %s\n", i, env[i]);
    }
    return 0;
}
