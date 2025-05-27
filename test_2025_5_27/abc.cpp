#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <string.h>

using namespace std;

// ls -a -b -c -d -e ---> 长字符串
// "ls" "-a" "-b" "-c" "-d" "-e"


// ./proc -a -b -c
int main(int argc, char* argv[])
{   
    if (argc != 2)
    {
        printf("Usage: \n\t%s [-a/-b/-c/-ab/-bc/-ac/-abc]\n", argv[0]);
        return 1;
    }

    if (strcmp("-a", argv[1]) == 0)
    {
        printf("功能a\n");
    }

    if (strcmp("-b", argv[1]) == 0)
    {
        printf("功能b\n");
    }

    if (strcmp("-c", argv[1]) == 0)
    {
        printf("功能c\n");
    }

    if (strcmp("-ab", argv[1]) == 0)
    {
        printf("功能ab\n");
    }

    if (strcmp("-abc", argv[1]) == 0)
    {
        printf("功能abc\n");
    }
    //for (int i = 0; i < argc; ++ i)
    //{
    //    printf("argv[%d]:-->%s\n", i, argv[i]);
    //}
    return 0;
}
