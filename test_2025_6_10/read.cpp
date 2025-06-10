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
    //int fd = open(FILE_NAME, O_WRONLY | O_CREAT | O_APPEND, 0666); // 以只写方式打开
    int fd = open(FILE_NAME, O_RDONLY); // 以只写方式打开
    if (fd < 0)
    {
        perror("open");
        return 1;
    }
      
    // 读取
    char buffer[1024];
    ssize_t num = read(fd, buffer, sizeof(buffer) - 1);
    if (num > 0)
    {
        buffer[num] = 0; // 
    }
    printf("%s", buffer); // 将读取的字节流转换为 C 风格的字符串（以 \0 结尾的字符数组）

    close(fd);
    return 0;
}
