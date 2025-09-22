#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "Util.hpp"
using namespace std;

const string sep = "\r\n";
const string default_root = "./wwwroot";
const string home_page = "index.html";
const string html_404 = "wwwroot/404.html";

// http请求
class HttpRequest
{
public:
    HttpRequest(){}
    ~HttpRequest(){}

    void parse()
    {
        // 1. 从inbuffer中拿到第一行, 分隔符 \r\n
        string line = Util::getOneLine(inbuffer, sep);
        if (line.empty()) return;

        // 2. 从请求行中提取三个字段
        //cout << "line: " << line << endl;
        stringstream ss(line); // 以空格作为分割符
        ss >> method >> url >> httpversion; // 把分割出的三个字符串写入

        // 2.1 /test.py?account=123456&passwd=000000
        // 通过 ? 将左右进行分离
        // 如果是POST方法, 本来就是分离的
        // 左边为PATH, 右边为parm


        // 3. 添加web默认路径
        path = default_root; // ./wwwroot
        path += url; // ./wwwroot/index.html
        if (path[path.size()-1] == '/') 
            path += home_page; 

        // 4. 获取path对应的资源后缀
        //  ./wwwroot/index.html
        //  ./wwwroot/test/a.html
        //  ./wwwroot/image/Bird.jpg
        auto pos = path.rfind("."); // 从路径的末尾开始, 往前找
        if (pos == string::npos)
            suffix = ".html";
        else 
            suffix = path.substr(pos); // .jpg

        // 5. 得到资源的大小
        // stat() 用来获取指定路径文件（或目录）的 状态信息，并将结果填充到一个 struct stat 结构体中。
        struct stat st;
        int n = stat(path.c_str(), &st);
        if (0 == n) // 成功
            size = st.st_size;
        else // 失败
            size = -1;
    }
public:
    string inbuffer;
    // string reqline; // 请求行
    // vector<string> reqhead; // 请求报头
    // string body; // 正文

    string method; // 请求方法
    string url; // 请求链接
    string httpversion; // http版本
    string path; // 资源路径
    string suffix; // 资源后缀
    int size; // 正文长度

    string parm; // 参数
};

// http响应
class HttpResponse
{
public:
    string outbuffer;
    // string respline; // 响应行
    // vector<string> resphead; // 响应报头
    // string body; // 正文
};