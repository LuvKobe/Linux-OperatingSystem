#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <string.h>

using namespace std;

#define MY_ENV "myval"

int main()
{ 
    char* myenv = getenv(MY_ENV);
	if (NULL == myenv)
	{
		printf("%s, not found\n", MY_ENV);
		return 1;
	}
	printf("%s=%s\n", MY_ENV, myenv);

    return 0;
}

