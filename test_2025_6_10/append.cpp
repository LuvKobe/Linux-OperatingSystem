#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

#define FILE_NAME "log.txt"

int main()
{
    int fd = open(FILE_NAME, O_WRONLY | O_CREAT | O_APPEND, 0666); // 以只写方式打开
    if (fd < 0)
    {
        perror("open");
        return 1;
    }
      
    // 写入
    int cnt = 5;
    
    char outBuffer[64];

    while (cnt)
    {
        sprintf(outBuffer, "%s:%d\n", "I LOVE YOU", cnt--); // 用于将格式化的数据写入到一个字符数组中
        // 不需要 +1。因为 strlen(outBuffer) 返回的是字符串长度（不包含 \0），
        // 而 write 是二进制安全的，它会写入指定长度的字节。
        // 如果 +1，会把字符串结束符 \0 也写入文件，可能导致文件末尾多出一个不可见字符。
        write(fd, outBuffer, strlen(outBuffer)); 
    }
    
    
    close(fd);
    return 0;
}
