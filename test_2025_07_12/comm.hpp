#ifndef _COMM_HPP_
#define _COMM_HPP_

#include <iostream>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

// key_t ftok(const char *pathname, int proj_id);
#define PATHNAME "." // 文件路径
#define PROJ_ID 0x66 // 项目标识符
#define  MAX_SIZE 4096

key_t getKey()
{
    key_t k = ftok(PATHNAME, PROJ_ID);
    if (k < 0)
    {   
        // cin --> stdin ---> 0
        // cout --> stdout ---> 1
        // cerr --> stderr ---> 2
        std::cerr << errno << " : " << strerror(errno) << std::endl;
        exit(1); // 终止进程
    }
    return k;
}

int getShmHelper(key_t k, int flags)
{
    // k是要通过shmget调用设置进共享内存属性中的！
    // 用来标识该共享内存在内核中的唯一性！

    // shmid 和 key 的区别
    // 相当于 fd 和 inode 的区别
    int shmid = shmget(k, MAX_SIZE, flags); // 创建共享内存
    if (shmid < 0) // 如果创建失败
    {
        std::cerr << errno << " : " << strerror(errno) << std::endl; // 输出失败的原因
        exit(2);
    }
    return shmid;
}

// 获取共享内存
int getShm(key_t k)
{
    return getShmHelper(k, IPC_CREAT);
}

// 创建共享内存
int createShm(key_t k)
{
    return getShmHelper(k, IPC_CREAT | IPC_EXCL | 0600); // 0600 --> 拥有者具有读写权限, 其他人没有
}

// 把共享内存贴到程地址空间上
void *attachShm(int shmid)
{
    void *mem = shmat(shmid, nullptr, 0); // 64位系统指针大小占8字节(window一般是4字节)
    if ((long long)mem == -1L)
    {
        std::cerr << "shmat:" << errno << " : " << strerror(errno) << std::endl; // 输出失败的原因
        exit(3); // 
    }
    return mem;
}

// 去掉进程和共享内存之间的映射关系
void detachShm(void* start)
{
    if (shmdt(start) == -1)
    {
        std::cerr << "shmdt:" << errno << " : " << strerror(errno) << std::endl; // 输出失败的原因
    }
}

// 删除共享内存
void delShm(int shmid)
{
    if (shmctl(shmid, IPC_RMID, nullptr) == -1)
    {
        std::cerr << errno << " : " << strerror(errno) << std::endl; // 输出失败的原因
    }
}

#endif