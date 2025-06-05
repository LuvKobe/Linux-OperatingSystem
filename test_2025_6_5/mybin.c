#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("PATH: %s\n", getenv("PATH"));
    printf("PWD: %s\n", getenv("PWD"));
    printf("MYENV: %s\n", getenv("MYENV"));

    printf("这是另一个 C 程序\n");
    
    return 0;
}
