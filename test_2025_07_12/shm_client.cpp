#include "comm.hpp"

int main()
{
    // 1. 创建
    key_t k = getKey();
    printf("key: 0x%x\n", k); // key
    int shmid = getShm(k);
    printf("shmid: %d\n", shmid);  // shmid

    //sleep(5);

    // 2. 挂接关联
    char *start = (char*)attachShm(shmid);
    printf("attach success, address start: %p\n", start);

    // 3. 开始通信
    const char* message = "hello server, 我是二号进程, 正在和你通信！";
    pid_t id = getpid(); // 二号进程的pid
    int cnt = 1;
    while (true)
    {   
        sleep(1); // 客户端每隔1s写一条
        // pid, cnt, message --> 发给客户端
        snprintf(start, MAX_SIZE, "%s[pid:%d][消息编号:%d]", message, id, cnt++); // 这个接口默认添加'\0'
    }

    // 4. 去掉关联
    detachShm(start);
    printf("detach success\n");

    // client不需要删除

    return 0;
}