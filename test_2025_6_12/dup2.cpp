#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>


int main()
{
    umask(0);
    //int fd = open("log.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    //int fd = open("log.txt", O_WRONLY | O_CREAT | O_APPEND, 0666); // 追加
    int fd = open("log.txt", O_RDONLY); // 追加
    if (fd < 0)
    {
        perror("open");
        return 1;
    }
    dup2(fd, 0); // 输入重定向

    char line[64];
    while (1)
    {
        printf(">");
        if (fgets(line, sizeof(line), stdin) == NULL) break; 
        printf("%s", line);
    }

    /*
    // 重定向
    dup2(fd, 1);

    // 操作
    printf("open fd: %d\n", fd);
    
    const char *msg = "hello edison\n";
    write(1, msg, strlen(msg));

    // 关闭文件
    fflush(stdout);
    close(fd);
    */

    return 0;
}
