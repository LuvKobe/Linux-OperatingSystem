#include "udpServer.hpp"
#include <memory>
#include <unordered_map>
#include <fstream>
#include <signal.h>
#include <stdio.h>

using namespace std;
using namespace Server;

const string dictTxt = "./dict.txt";
unordered_map<string, string> dict;

static void Usage(string proc)
{
    cout << "\nUsage:\n\t" << proc << " local_port\n\t" << endl;
}

static bool cutString(const string &target, string *s1, string *s2, const string &sep)
{
    // apple:苹果
    auto pos = target.find(sep);
    if (pos == string::npos) return false;
    *s1 = target.substr(0, pos); // [apple:)
    *s2 = target.substr(pos + sep.size());
    return true;
}

//
static void initDict()
{
    ifstream in(dictTxt, ios::binary);
    if (!in.is_open())
    {
        cerr << "open file " << dictTxt << " error" << endl;
        exit(OPEN_ERR);
    }
    
    //
    string line;
    string key, value;
    while (getline(in, line))
    {
        //cout << line << endl;
        if (cutString(line, &key, &value, ":"))
        {
            dict.insert(make_pair(key, value));
        }
    }

    in.close();
    cout << "load dict success..." << endl;
}

// 捕捉信号
void reload(int signo)
{
    (void)signo;
    initDict();
}

//
static void debugPrint()
{
    for (auto &dt : dict)
    {
        cout << dt.first << " # " << dt.second << endl;
    }
}

// 处理client发来的数据(demo1)
void handlerMsg(int sockfd, string clientip, uint16_t clientport, string msg)
{
// 对msg进行特定的业务处理, 而不关心msg怎么来的故完成了server通信和业务逻辑的解耦
    string response_msg;
    auto iter = dict.find(msg);
    if (iter == dict.end()) response_msg = "unknown";
    else response_msg = iter->second; // 如果找到了, 就返回中文

    // 把消息开始返回给客户端
    struct sockaddr_in client;
    bzero(&client, sizeof(client));

    client.sin_family = AF_INET;
    client.sin_port = htons(clientport);
    client.sin_addr.s_addr = inet_addr(clientip.c_str());

    sendto(sockfd, response_msg.c_str(), response_msg.size(), 0, (struct sockaddr*)&client, sizeof(client));
}

// 处理client发来的命令(demo2)
void execCommand(int sockfd, string clientip, uint16_t clientport, string cmd)
{
    // 先判断cmd中是否有rm
    auto end = cmd.find("rm"); // 在字符串 cmd 中查找子串 "rm" 出现的位置。
    // npos是一个特殊常量，表示“没找到”。
    // 如果找到 "rm"（即 end 不是 npos），就 return;
    if (end != string::npos) 
    {
        cerr << clientip << ":" << clientport << " 正在做一个非法的操作!" << endl;
        return;
    } 

    // 1.cmd解析: ls -a -l
    string response;
    FILE *fp = popen(cmd.c_str(), "r");
    if (fp == nullptr) response = cmd + " exec failed";
    
    // 从文件中读
    char line[1024]; // 缓冲区
    while (fgets(line, sizeof(line), fp))
    {
        response += line; // 把读到的结果拼到line中
    }
    pclose(fp);

    // 把消息开始返回给客户端
    struct sockaddr_in client;
    bzero(&client, sizeof(client));

    client.sin_family = AF_INET;
    client.sin_port = htons(clientport);
    client.sin_addr.s_addr = inet_addr(clientip.c_str());

    sendto(sockfd, response.c_str(), response.size(), 0, (struct sockaddr*)&client, sizeof(client));
}

/*
./udpServer ip port
./udpServer 172.19.61.204(绑定服务器的内网ip) 8080
sudo netstat -nuap
*/
int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }
    uint16_t port = atoi(argv[1]);
    //string ip = argv[1];

    // 往dict.txt添加文件能够实时生效, 而不重启服务器, 那么需要做一个信号捕捉
/*
    此时不需要重启服务器, 只需要Ctrl+C一下即可
    127.0.0.1[34170]# goodman
    ^Cload dict success...
    127.0.0.1[34170]# goodman
*/
    //signal(2, reload);
    //initDict();
    //debugPrint();
    
    //unique_ptr<udpServer> usvr(new udpServer(handlerMsg, port));
    unique_ptr<udpServer> usvr(new udpServer(execCommand, port));


    usvr->initServer();
    usvr->startServer();

    return 0;
}