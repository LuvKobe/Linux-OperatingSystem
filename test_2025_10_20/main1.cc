#include "util.hpp"

using func_t = function<void ()>; // 设置回调函数

#define INIT(v) do{\
            cbs.push_back(printLog);\
        } while(0)

#define EXEC_OTHER(cbs) do{\
            for (auto const &cb : cbs) cb();\
        } while(0)

int main()
{
    vector<func_t> cbs;
    INIT(cbs);

    setNonBlock(0); // 设置非阻塞
    char buffer[1024];
    while (true)
    {
        ssize_t s = read(0, buffer, sizeof(buffer)-1);
        if (s > 0)
        {
            buffer[s-1] = 0;
            cout << "echo# " << buffer << endl;
        }
        else if (s == 0)
        {
            cout << "read end" << endl;
            break;
        }
        else
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK) // 说明是非阻塞, 只是数据没有准备好
            {
                cout << "我没错, 只是没数据" << endl;
                EXEC_OTHER(cbs);
            }
            else if (errno == EINTR) // 此时说明是 中断的系统调用, 那么就继续
            {
                continue;
            }
            else // 除了上面两种, 其余全是真正的错误
            {
                cout << "return value : " << s << ", errno : " << strerror(errno) << endl; // 可以看到返回值为-1
                break;
            }
        }
        sleep(1);        
    }
    return 0;
}
