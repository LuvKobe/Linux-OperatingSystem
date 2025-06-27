#include <iostream>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <cassert>
#include <sys/wait.h>
#include <sys/types.h>
using namespace std;

// 父进程进行读取, 子进程进行写入
int main()
{
  // 第一步：创建管道文件，打开读写端
  int fds[2];
  int n = pipe(fds);
  assert(n == 0);

  // 第二步：fork
  pid_t id = fork();
  assert(id >= 0);
  if (0 == id)
  {
    // 子进程进行写入, 关闭fd[0]
    close(fds[0]);

    // 子进程的通信代码
    const char *s = "我是子进程, 我正在给你发消息";
    int cnt = 0;
    while (true) // 让子进程不断的去给父进程写入
    {
      cnt++;
      char buffer[1024]; // 目前缓冲区的数据只有子进程能看到
      snprintf(buffer, sizeof buffer, "child-->parent say: %s[%d][%d]", s, cnt, getpid());

      // 往文件写入
      write(fds[1], buffer, strlen(buffer));
      cout << "count: " << cnt << endl;
    }
    // 子进程退出以后, 那么打开的相关的文件描述符也一定会被自动关闭的
    close(fds[1]);
    cout << "子进程关闭自己的写端" << endl;
    //sleep(100000);
    exit(0);
  }
  // 父进程进行读取, 关闭fd[1]写入
  close(fds[1]);

  // 父进程的通信代码
  while (true)
  {
    sleep(1);
    char buffer[1024];
    ssize_t s = read(fds[0], buffer, sizeof(buffer) - 1);
    if (s > 0)
    {
      buffer[s] = 0;
      cout << "Get message# " << buffer << " | 我是父进程,我正在接收消息[" << getpid() << "]" << endl;
    }
    else
    {
      // 读到文件结尾
      cout << "read: " << s << endl;
      break;
    }
    break;
  }
  close(fds[0]);
  cout << "父进程关闭读端" << endl;
  // 等待子进程退出
  int status = 0;
  n = waitpid(id, &status, 0);
  assert(n == id);

  cout << "pid->" << n << " : " <<  (status & 0x7F) << endl;

  return 0;
}
