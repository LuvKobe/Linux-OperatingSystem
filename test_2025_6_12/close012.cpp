#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>


int main()
{
    close(1);
    //close(2);
    umask(0);
    int fd = open("log.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd < 0)
    {
        perror("open");
        return 1;
    }

    // 操作
    printf("open fd: %d\n", fd);

    // 关闭文件
    fflush(stdout);
    close(fd);
    
    return 0;
}
