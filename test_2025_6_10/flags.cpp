#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define FILE_NAME "log.txt"

// 每一个宏对应的数值只有一个比特位是1, 彼此位置不重叠
#define ONE (1 << 0) // 0x1
#define TWO (1 << 1) // 0x2
#define THREE (1 << 2) //0x4
#define FOUR (1 << 3) // 0x8

void show(int flags)
{
    if (flags & ONE) printf("one\n");
    if (flags & TWO) printf("two\n");
    if (flags & THREE) printf("three\n");
    if (flags & FOUR) printf("four\n");
}

int main()
{ 
    show(ONE);
    printf("------------------\n");
    show(TWO);
    printf("------------------\n");
    show(ONE | TWO);
    printf("------------------\n");
    show(ONE | TWO | THREE);
    printf("------------------\n");
    show(ONE | TWO | THREE | FOUR);

    return 0;
}
