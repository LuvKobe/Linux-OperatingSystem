#include "comm.hpp"

// server进行管道的管理
// server从文件中读取

int main()
{
    // 创建管道
    bool ret = createFifo(NAMED_PIPE);
    assert(ret);
    (void)ret;

    // 开始通信, 打开文件 
    cout << "Server begin" << endl;
    int rfd = open(NAMED_PIPE, O_RDONLY); // 以读方式打开
    cout << "Server end" << endl;
    if (rfd < 0)
    {
        exit(1);
    }

    // read
    char buffer[1024];
    while (true)
    {
        ssize_t s = read(rfd, buffer, sizeof(buffer)-1);
        if (s > 0) // 读取成功
        {
            buffer[s] = 0;
            cout << "Client ---> Server# " << buffer << endl;
        }
        else if (s == 0)
        {
            cout << "Client quit, me too!" << endl;
            break;
        }
        else // 读取出错了
        {
            cout << "error string: " << strerror(errno) << endl;
            break;
        }
    }

    close(rfd);

    // 删除管道
    // sleep(10); 
    removeFifo(NAMED_PIPE);
    return 0;
}