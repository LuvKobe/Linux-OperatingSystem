#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>

using namespace std;

int main()
{
  // 创建子进程
  pid_t id = fork();
  
  if (id == 0)
  {
    // 子进程
    while (1)
    {  
      printf("子进程的pid: %d, 它的ppid: %d, 它的id是: %d\n", getpid(), getppid(), id);
      sleep(1);
    }
  }
  else if (id > 0)
  {
    // 父进程
    while (1)
    { 
      printf("父进程的pid: %d, 它的ppid: %d, 它的id是: %d\n", getpid(), getppid(), id);
      sleep(2);
    }
  }
  else {
    ;
  }

  //printf("我的pid: %d, 我的父进程ppid: %d, id是: %d\n", getpid(), getppid(), id);
  
  sleep(2);
  
  return 0;
}
