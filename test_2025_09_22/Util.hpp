#pragma once

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class Util
{
public:
    // GET /a/b/c HTTP/1.1\r\nHost: 8.156.65.211:8082
    static string getOneLine(string &buffer, const string &sep)
    {
        auto pos = buffer.find(sep);
        if (pos == string::npos) return "";
        string sub = buffer.substr(0, pos);
        //buffer.erase(0, sub.size() + sep.size());
        return sub; // 返回: GET /a/b/c HTTP/1.1
    }
    // 打开文件, 读取内容, 放到out中
    static bool readFile(const string resource, string* out)
    {
        ifstream in(resource);
        if (!in.is_open()) return false; // 资源不存在, 打开失败
        
        // 打开成功, 循环进行读取
        string line;
        while (getline(in, line))
        {
            *out += line;
        } 
        // 关闭
        in.close();
        return true;
    }
};