#pragma once

#include <iostream>
#include <string>
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
        buffer.erase(0, sub.size() + sep.size());
        return sub; // 返回: GET /a/b/c HTTP/1.1
    }
};