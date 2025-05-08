#include <iostream>
#include <unistd.h>
using namespace std;

int main()
{
  while (1)
  {
    cout << "我是一个进程" << endl;
    sleep(1);
  }

  return 0;
}
