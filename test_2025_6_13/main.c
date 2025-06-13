#include "myStdio.h" 
#include <stdio.h>

int main()
{
    FILE_ *fp = fopen_("./log.txt", "w");
    if (fp == NULL)
      return 1;
    
    int cnt = 10;
    const char* msg = "hello edison ";
    while (1)
    {   
        fwrite_(msg, strlen(msg), fp);
        fflush_(fp);
        sleep(1); // 每隔一秒写一次
        printf("count is %d\n", cnt);
        //if (cnt == 5) fflush_(fp);
        cnt--;

        if (0 == cnt) break;
    }

    fclose_(fp);
}
