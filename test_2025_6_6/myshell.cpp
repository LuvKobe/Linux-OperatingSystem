#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <string.h>

#define NUM 1024
#define OPT_NUM 64

char lineCommand[NUM];
char *myargv[OPT_NUM]; // 指针数组
int lastCode = 0;
int lastSignal = 0;

int main()
{
    while (1)
    {
        // 1. 输出提示符
        printf("[edison@vm-centos:~]# ");
        fflush(stdout); // 上面的printf没有加'\n', 所以要用fflush刷新缓冲区

        // 2. 获取用户输入(输入的时候，自己会按回车, 相当于输入了'\n') 
        char *s = fgets(lineCommand, sizeof(lineCommand) - 1, stdin);
        assert(NULL != s);
        (void)s;
    
        // 清楚最后一个'\n'
        lineCommand[strlen(lineCommand) - 1] = 0; // 把最后一个元素'\n'赋值为0
        //printf("%s\n", lineCommand);
    
        // 3. 字符串切割
        myargv[0] = strtok(lineCommand, " ");
        int i = 1;
        if (myargv[0] != NULL && strcmp(myargv[0], "ls") == 0) // 给ls命令添加颜色
        {
            myargv[i++] = (char*)"--color=auto";
        }


        // 如果没有字串了, strtok会返回NULL, 也就是myargv[end] = NULL 
        //int i = 1;
        while (myargv[i ++] = strtok(NULL, " "))
        {
            // 测试是否成功
            ;
        }
    
        // 判定如果是cd命令, 则不需要创建子进程, 直接让shell自己执行对应的命令
        // 其实本质就是执行系统接口
        // 像这种不需要让我们的子进程来执行, 而是让shell自己执行的命令, 称之为: 内建/内置命令
        if (myargv[0] != NULL && strcmp(myargv[0], "cd") == 0)
        {
            if (myargv[1] != NULL)
            {
                chdir(myargv[1]);
            }
            continue;
        }
        
        // 
        if (myargv[0] != NULL && myargv[1] != NULL && strcmp(myargv[0], "echo") == 0)
        {
            if (strcmp(myargv[1], "$?") == 0)
            {
                printf("ExitCode: %d, SignalCode: %d\n", lastCode, lastSignal);
            }
            else 
            {
                printf("%s\n", myargv[1]);
            }
            continue;
        }

        // 测试是否成功(条件编译)
#ifdef DEBUG 
        for (int i = 0; myargv[i]; i ++)
        {
            printf("myargv[%d]: %s\n", i, myargv[i]);
        }
#endif
        
        // 4. 执行命令
        pid_t id = fork();
        assert(-1 != id);

        if (id == 0)
        {
            // child --> 执行程序替换
            execvp(myargv[0], myargv);
            exit(111);
        }

        // parent
        int status = 0;
        pid_t ret = waitpid(id, &status, 0);
        assert(ret > 0);
        (void)ret;
        lastCode = (status >> 8) & 0xFF; // 退出码
        lastSignal = (status & 0x7F); // 退出信号
        
    }
    return 0;
}
