#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "Util.hpp"
using namespace std;

const string sep = "\r\n";
const string default_root = "./wwwroot";
const string home_page = "index.html";

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
        cout << "line: " << line << endl;
        stringstream ss(line); // 以空格作为分割符
        ss >> method >> url >> httpversion; // 把分割出的三个字符串写入

        // 3. 添加web默认路径
        path = default_root; // ./wwwroot
        path += url; // ./wwwroot/index.html
        if (path[path.size()-1] == '/') path += home_page; 
    }
public:
    string inbuffer;
    // string reqline; // 请求行
    // vector<string> reqhead; // 请求报头
    // string body; // 正文

    string method; // 请求方法
    string url; // 请求链接
    string httpversion; // http版本
    string path;
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