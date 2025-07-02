#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <cassert>
#include <csignal>
#include <sys/wait.h>
#include <sys/types.h>
using namespace std;

#define PROCSS_NUM 5

//#define MakeSeed() srand((unsigned long)time(nullptr) ^ getpid() ^ 0x171328 ^ rand()%1234);

//////////////////////////////////////////////////////////////////////////下面的代码是子进程要完成的某种任务(模拟一下)
// 函数指针类型
typedef void(*func_t)(); //func_t 是一个指向无参数、无返回值函数的指针类型

void upLoadTask()
{
    cout << "子进程[" << getpid() << "] 正在执行--> " << "上传任务" << endl;
    cout << endl;
    sleep(1);
}

void downLoadTask()
{
    cout << "子进程[" << getpid() << "] 正在执行--> " << "下载任务" << endl;
    cout << endl;
    sleep(1);
}

void deleteTask()
{
    cout << "子进程[" << getpid() << "] 正在执行--> " << "删除任务" << endl;
    cout << endl;
    sleep(1);
}

void ioTask()
{
    cout << "子进程[" << getpid() << "] 正在执行--> " << "IO任务" << endl;
    cout << endl;
    sleep(1);
}

void flushTask()
{
    cout << "子进程[" << getpid() << "] 正在执行--> " << "刷新任务" << endl;
    cout << endl;
    sleep(1);
}

// 把所有的任务插入进任务数组表中
void loadTaskFunc(vector<func_t> *funcMap)
{
    assert(funcMap);
    funcMap->push_back(move(upLoadTask));
    funcMap->push_back(move(downLoadTask));
    funcMap->push_back(move(deleteTask));
    funcMap->push_back(move(ioTask));
    funcMap->push_back(move(flushTask));
}


//////////////////////////////////////////////////////////////////////////下面的代码是一个多进程文件

// 给每一套子进程+管道取名
class subEp // Endpoint
{
public:
    // 拷贝构造 + 初始化列表
    subEp(pid_t subId, int writeFd)
        : m_subId(subId), m_writeFd(writeFd)
    {
        char nameBuffer[1024];
        snprintf(nameBuffer, sizeof nameBuffer, "%d 号子进程[pid(%d)——fd(%d)]", num++, m_subId, m_writeFd);
        m_name = nameBuffer;
    }

    // 添加公有访问器
    // m_name 是 subEp 类的私有成员，外部函数无法直接访问私有成员。
    const string& getName() const 
    {
        return m_name;
    }

    pid_t getSubId() const
    {
        return m_subId;
    }

    int getWriteFd() const
    {
        return m_writeFd;
    }
private:
    static int num;
    string m_name; // 名字
    pid_t m_subId; // 子进程pid
    int m_writeFd; // 写操作符
};
int subEp::num = 1;

// 读取任务的方法
int recvTask(int readFd)
{
    int code = 0;
    ssize_t s = read(readFd, &code, sizeof code);

    if (s == 4)
        return code;
    else if (s <= 0)
        return -1;
    else 
        return 0;
}

// 发生任务的方法
void sendTask(const subEp& processChild, int taskNum)
{
    cout << "父进程发送task num " << taskNum 
        << " ---> send to ---> " << processChild.getName() << endl;
    // 往管道写入
    int n = write(processChild.getWriteFd(), &taskNum, sizeof(taskNum));
    assert(n == sizeof(int)); // 保证发送成功
    (void)n;
}

/*
你要把我们的子进程给创建好，管道各方面都给我建立好，
建立好之后把所有的管道信息，都给我保存到这个subs这个结构里面。
*/
void createSubProcess(vector<subEp> *subs, const vector<func_t> &funcMap)
{
    vector<int> deleteFd; // 保存每个子进程的写端文件描述符
    // 创建5个子进程
    for (size_t i = 0; i < PROCSS_NUM; ++i)
    {
        int fds[2];
        int n = pipe(fds);
        assert(0 == n);
        (void)n;
        // 父进程打开的文件会被子进程共享
        pid_t id = fork();
        if (id == 0)
        {
            /* 第二次创建子进程的时候, 这个deleteFd数组里面保存的是第一个子进程的写端文件描述符
               那么我们直接关闭第一个子进程的写端文件描述符。
               第三次创建子进程的时候, 数组里面保存的是1号和2号子进程的写端文件描述符
               为了防止被3号子进程继承, 所以我们先关闭1号和2号子进程的写端文件描述符
               此时就能创建出 1对1的管道!
            */
            for (int i = 0; i < deleteFd.size(); ++i)
            {
                close(deleteFd[i]);
            }

            // child --> 进行处理任务
            close(fds[1]); // 子进程关闭写操作(保留读操作0)

            /*
            我们的子进程它不断的在进行任务的读取，
            而父进程它就不断的选择子进程，向子进程发送我们对应的任务码，
            我们的子进程读到这个任务码，然后直接调用曾经设定好的任务函数，
            */
            while (true)
            {
                // 1) 子进程获取命令码, 父进程如果没有发生命令码, 那么子进程就该阻塞等待
                // 它能够通过fds[0]管道的独端，读取到父进程刚刚发送的这个整数
                int commandCode = recvTask(fds[0]);

                // 2) 子进程成功得到命令码, 那么就根据命令码来完成任务
                if (commandCode >= 0 && commandCode < funcMap.size())
                {
                    funcMap[commandCode]();
                }
                else if (commandCode == -1) // 如果等于-1, 说明不想让子进程继续运行了
                {
                    break; // 跳出循环以后, 子进程就退出了, 退出之后调用exit直接终止自己
                }
                // cout << "sub recv code error!" << endl;
            }
            exit(0);
        }
        // father --> 进行处理任务
        close(fds[0]);              // 父进程关闭读操作(保留写操作1)
        subEp sub(id, fds[1]);      // 构建一个子进程对象
        subs->push_back(move(sub)); // 插入到数组中
        /*
        这里的 move(sub) 将 sub 对象转换为右值引用，
        触发 vector 的移动构造函数，将 sub 的资源所有权转移到 vector 中，而不是进行深拷贝。
        */
        deleteFd.push_back(fds[1]); // 保存每个子进程的写端文件描述符
    }
}

// 让父进程 负载均衡的去给子进程下发任务
void loadBlanceControl(const vector<subEp> &subs, const vector<func_t> &funcMap, int taskCnt)
{
    int procNum = subs.size(); // 获取子进程个数
    int taskNum = funcMap.size(); // 获取任务的个数
    //bool quit = false;
    bool forever = (taskCnt == 0 ? true : false);
    while (true)
    {
        // 1) 先选择一个子进程 --> 相当于在subs数组中选择一个下标
        // 我们将任务均衡的下发给每一个子进程, 让子进程负载均衡(采用随机数方法)
        int subIdx = rand() % procNum;

        // 2) 选择一个任务 --> 相当于在funcMap方法表中选择一个下标
        int taskIdx = rand() % taskNum;

        // 3) 把任务发送给选择的子进程 --> 把对应任务的下标发送给子进程, 子进程读到以后就可以给我们执行
        sendTask(subs[subIdx], taskIdx);

        sleep(1);

        if (!forever)
        {
            taskCnt--;
            if (taskCnt == 0)
                break;
        }
    }
    // 如果写端退出, 那么读端会直接读到0
    for (int i = 0; i < procNum; ++ i)
    {
        close(subs[i].getWriteFd()); // 关闭所有写端
    }
}

#define TASK_QUIT -1

// 清理子进程资源
void cleanupChildProcess(const vector<subEp>& subs)
{
    // 等待所有子进程退出
    for (const auto& procChild : subs) {
        waitpid(procChild.getSubId(), nullptr, 0);
        cout << "子进程[" << procChild.getSubId() << "] 已退出" << endl;
    }
}

// 随机数方法
void makeRandSeed()
{
    srand((unsigned long)time(nullptr) ^ getpid() ^ 0x171328 ^ rand()%1234);
}

int main()
{
    makeRandSeed();

    // 1. 建立子进程, 并建立和子进程通信的信道
    
     // 1.1 创建任务表
    vector<func_t> funcMap;
    loadTaskFunc(&funcMap); // 把所有的任务加载进去
    
     // 1.2 创建子进程, 并且维护好父子进程的通信信道
     // 键值对 --> { 子进程id, wfd };
    vector<subEp> subs; // 所有子进程的信息都会保存在subs中
    createSubProcess(&subs, funcMap);

    // 2. 走到这里的就是父进程, 开始控制子进程, 负载均衡的向子进程发送命令码
    int taskCnt = 6; // 0 表示永远执行
    loadBlanceControl(subs, funcMap, taskCnt);

    // 3. 回收子进程信息
    cleanupChildProcess(subs);
    return 0;
}
