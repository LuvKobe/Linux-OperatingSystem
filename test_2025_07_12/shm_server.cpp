#include "comm.hpp"

int main()
{   
    // 1. 创建
    key_t k = getKey();
    printf("key: 0x%x\n", k); // key
    int shmid = createShm(k);
    printf("shmid: %d\n", shmid);  // shmid

    //sleep(5);

    // 2. 挂接关联
    char *start = (char*)attachShm(shmid);
    printf("attach success, address start: %p\n", start);

    // 3. 开始通信
    int cnt = 0;
    while (true)
    {
        printf("client say#  %s\n", start);
        struct shmid_ds ds;
        shmctl(shmid, IPC_STAT, &ds);
        printf("获取属性 --> size: %d, pid: %d, myself: %d, key: 0x%x\n", ds.shm_segsz, ds.shm_cpid, getpid(), ds.shm_perm.__key);
        sleep(1); // 服务端每隔1s读一条
        cnt++;
        if (cnt == 8) // 读10条消息就退出
            break;
    }

    // 4. 去掉关联
    detachShm(start);
    printf("detach success\n");
    
    sleep(10);

    // 5. 删除
    delShm(shmid);

    return 0;
}