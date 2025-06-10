#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

#define FILE_NAME(number) "log.txt"#number

int main()
{
    printf("stdin->fd: %d\n", stdin->_fileno);
    printf("stdout->fd: %d\n", stdout->_fileno);
    printf("stderr->fd: %d\n", stderr->_fileno);
    int fd0 = open(FILE_NAME(1), O_WRONLY | O_CREAT | O_APPEND, 0666);
    int fd1 = open(FILE_NAME(2), O_WRONLY | O_CREAT | O_APPEND, 0666);
    int fd2 = open(FILE_NAME(3), O_WRONLY | O_CREAT | O_APPEND, 0666);
    int fd3 = open(FILE_NAME(4), O_WRONLY | O_CREAT | O_APPEND, 0666);
    int fd4 = open(FILE_NAME(5), O_WRONLY | O_CREAT | O_APPEND, 0666);
    //int fd = open(FILE_NAME, O_WRONLY | O_CREAT | O_APPEND, 0666); // 以只写方式打开
    /*int fd = open(FILE_NAME, O_RDONLY); // 以只写方式打开
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
    */
    
    printf("fd: %d\n", fd0);
    printf("fd: %d\n", fd1);
    printf("fd: %d\n", fd2);
    printf("fd: %d\n", fd3);
    printf("fd: %d\n", fd4);

    close(fd0);
    close(fd1);
    close(fd2);
    close(fd3);
    close(fd4);
    return 0;
}
