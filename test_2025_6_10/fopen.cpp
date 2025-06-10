#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define FILE_NAME "log.txt"

int main()
{
    //FILE* fp = fopen(FILE_NAME, "w");
    //FILE* fp = fopen(FILE_NAME, "r");
    FILE* fp = fopen(FILE_NAME, "a");
    if (NULL == fp)
    {
        perror("open");
        return 1;
    }
    
    /*
    char buffer[64];
    while (fgets(buffer, sizeof(buffer) - 1, fp) != NULL)
    {
        buffer[strlen(buffer) - 1] = 0; // 移除从文件中读取的行末尾的换行符'\n'
        puts(buffer);
    }
    */
 
    int cnt = 5;
    while (cnt)
    {
        fprintf(fp, "%s:%d\n", "I LOVE YOU", cnt--);
    }

    // 关闭文件
    fclose(fp);
}
