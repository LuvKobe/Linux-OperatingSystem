#include "comm.hpp"

// client也打开同一份文件
// client 向文件中写入

int main()
{
    // 开始通信, 打开文件 
    cout << "Cerver begin" << endl;
    int wfd = open(NAMED_PIPE, O_WRONLY); // 以写方式打开
    cout << "Cerver end" << endl;
    if (wfd < 0)
    {
        exit(1);
    }

    // write
    char buffer[1024];
    while (true)
    {
        cout << "Please Say# ";
        // 按行读取
        fgets(buffer, sizeof(buffer)-1, stdin);
        // hello\n --> hello
        if (strlen(buffer) > 0)
        {
            buffer[strlen(buffer) - 1] = 0; //把字符串末尾的字符去除, 一般是用于删除换行符
        }
        ssize_t n = write(wfd, buffer, strlen(buffer));
        assert(n);
        (void)n;
    }

    close(wfd);
    return 0;
}