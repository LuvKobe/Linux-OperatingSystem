#include "selectServer.hpp"
#include "err.hpp"
#include <memory>

//using namespace std;
using namespace select_ns;

// 手册
static void Usage(string proc)
{
    cout << "\nUsage:\n\t" << proc << " port\n\t" << endl;
}

string transaction(const string &request)
{
    return request;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }
    uint16_t port = atoi(argv[1]);

    //创建一个堆上分配的 SelectServer 对象，并用 unique_ptr 智能指针管理它。
    unique_ptr<SelectServer> svr(new SelectServer(transaction, port));
    svr->initServer();
    svr->start();

    return 0;
}


/*
unique_ptr 是 C++11 引入的一种独占所有权智能指针，特点如下：
| 特性     | 说明                          |
| ------ | --------------------------- |
| 所有权唯一  | 不能被复制，只能通过 `std::move` 转移。  |
| 自动释放内存 | 离开作用域时自动调用 `delete`，防止内存泄漏。 |
| 轻量高效   | 无引用计数，性能与原始指针接近。            |
*/