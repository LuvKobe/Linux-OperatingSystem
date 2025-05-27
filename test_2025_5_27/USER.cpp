#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <string.h>

using namespace std;

#define USER "USER"

int main()
{
    char *who = getenv(USER);
    if (strcmp(who, "root") == 0)
    {
        printf("user: %s\n", who);
        printf("user: %s\n", who);
        printf("user: %s\n", who);
        printf("user: %s\n", who);
        printf("user: %s\n", who);
        printf("user: %s\n", who);
    }
    else 
    {
        printf("权限不足!\n");
    }
    return 0;
}
