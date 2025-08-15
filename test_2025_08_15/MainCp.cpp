#include "BlockQueue.hpp"
#include "Task.hpp"
#include <ctime>
#include <sys/types.h>
#include <unistd.h>


/*

流程：
    1.生产者派发任务  -->  |___阻塞队列(计算任务)___|  --> 2.消费者1处理任务  -->  |___阻塞队列(存储任务)___|  -->  3.消费者2记录任务结果,并写入文件中
*/

// C: 计算任务
// S: 存储任务
template<class C, class S>
class BlockQueues
{
public:
    BlockQueue<C> *c_bq;
    BlockQueue<S> *s_bq;
};

// 生产者: 生产计算任务
void* producter(void* _bqs)
{
    BlockQueue<CalTask> *bq = (static_cast< BlockQueues<CalTask, SaveTask>* >(_bqs))->c_bq;
    while (true)
    {
        // 生成活动
        int x = rand() % 10 + 1; // 在这里先用随机数构建一个数据
        int y = rand() % 5;
        int operCode = rand() % oper.size(); // 随机的操作符

        CalTask t(x, y, oper[operCode],mymath);
        bq->push(t); // 放入队列中
        cout << "producter thread 生产计算任务: " << t.toTaskString() << endl;
        sleep(1);
    }
    return nullptr;
}

// 消费者: 1. 先从计算阻塞队列中获取任务, 然后完成计算
//        2. 此时再把计算出来的结果, 推送到存储阻塞队列中
void* consumer(void* _bqs)
{
    BlockQueue<CalTask> *bq = (static_cast< BlockQueues<CalTask, SaveTask>* >(_bqs))->c_bq;
    BlockQueue<SaveTask> *save_bq = (static_cast< BlockQueues<CalTask, SaveTask>* >(_bqs))->s_bq;

    // 消费活动
    while (true)
    {
        // 计算任务
        CalTask t;
        bq->pop(&t); // 获取对头数据
        string ret = t();
        cout << "cal thread 完成计算任务: " << ret << " ... done" << endl;

        // 存储任务
        SaveTask save(ret, Save);
        save_bq->push(save);
        cout << "cal thread 推送存储任务完成..." << endl; 
    }
    return nullptr;
}


// 保存者: 从存储阻塞队列中获取结果, 然后去执行对应的任务回调方法
void *saver(void* _bqs)
{
    BlockQueue<SaveTask> *save_bq = (static_cast< BlockQueues<CalTask, SaveTask>* >(_bqs))->s_bq;

    // 从阻塞队列中获取保存好的任务
    while (true)
    {
        SaveTask t;
        save_bq->pop(&t);
        t();
        cout << "save thread 保存任务完成..." << endl;
    }

    return nullptr;
}

// while :; do ps -aL | grep myproc; sleep 1; echo "--------------------------"; done

int main()
{
    srand((unsigned long)time(nullptr) ^ getpid());

    BlockQueues<CalTask, SaveTask> bqs;

    bqs.c_bq = new BlockQueue<CalTask>(); // 构造一个计算阻塞队列的对象
    bqs.s_bq = new BlockQueue<SaveTask>(); // 构造一个存储阻塞队列的对象

    pthread_t p, c, s; // p:生产者 c:消费者&&生产者 c:保存者

    // 让三个线程看到同一份阻塞队列
    pthread_create(&p, nullptr, producter, &bqs);
    pthread_create(&c, nullptr, consumer, &bqs);
    pthread_create(&s, nullptr, saver, &bqs);


    pthread_join(c, nullptr);
    pthread_join(p, nullptr);
    pthread_join(s, nullptr);

    delete bqs.c_bq;
    delete bqs.s_bq;
    return 0;
}