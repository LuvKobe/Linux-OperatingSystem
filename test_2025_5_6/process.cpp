#include "process.h"
using namespace std;

#define NUM 101

void ProncessOn() //function define
{
  int cnt = 0;
  char bar[NUM];
  memset(bar, '\0', sizeof(bar)); // 缓冲区清零
  
  // 旋转光标 - 逆时针
  const char *lable = "|\\-/";

  // 循环 101 次
  while (cnt <= 100)
  {
    //printf("[%-100s][%-3d%%][%c]\r", bar, cnt, lable[cnt % 4]);
    printf("\033[42;34m[%-100s][%-3d%%][%c]\033[0m\r", bar, cnt, lable[cnt % 4]);
    fflush(stdout);
    bar[cnt++] = '*';
    //sleep(1);
    usleep(50000); 
  }

  printf("\n");
}
