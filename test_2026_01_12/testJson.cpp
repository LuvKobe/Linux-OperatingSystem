#include <iostream>
#include <jsoncpp/json/json.h>

/*
Json中有3个重要的类
Value
Reader
Writer
*/

// 序列化函数：把 Json::Value 转成字符串
void serializeJson()
{
    // 1. 构造 JSON 数据 
    Json::Value root;
    Json::Value item1;
    item1["key1"] = "value1";
    item1["key2"] = "value2";

    Json::Value item2;
    item2["key3"] = "value3";
    item2["key4"] = "value4";

    root.append(item1);
    root.append(item2);

    // 2. 序列化
    Json::StyledWriter writer;
    std::string jsonStr = writer.write(root);
    std::cout << "序列化后的 JSON:\n" << jsonStr << std::endl;
}


// 反序列化函数：把字符串转成 Json::Value
void deserializeJson()
{
    // 假设这是接收到的 JSON 字符串（从文件/网络读取都行）
    std::string jsonStr = R"([
        {"key1":"value1","key2":"value2"},
        {"key3":"value3","key4":"value4"}
    ])";

    // 创建 JSON 解析器
    Json::Reader reader;
    Json::Value root;

    // 反序列化：将字符串解析成 JSON 对象
    if (!reader.parse(jsonStr, root))
    {
        std::cout << "解析 JSON 失败！" << std::endl;
    }

    // 遍历解析后的结果
    std::cout << "\n反序列化结果：" << std::endl;
    for (int i = 0; i < root.size(); ++i)
    {
        const Json::Value &item = root[i];
        for (auto it = item.begin(); it != item.end(); ++it)
        {
            std::cout << it.key().asString() << ": " << it->asString() << std::endl;
        }
        std::cout << "-----" << std::endl;
    }
}

int main()
{
    serializeJson();
    deserializeJson();
    return 0;
}

// 编译运行：g++ testJson.cpp -ljsoncpp

/*
[edison@vm-centos:~/myCode/0_Cpp11]$ g++ testJson.cpp -ljsoncpp
[edison@vm-centos:~/myCode/0_Cpp11]$ ./a.out 
[
   {
      "key1" : "value1",
      "key2" : "value2"
   },
   {
      "key3" : "value3",
      "key4" : "value4"
   }
]
*/


/*
[edison@vm-centos:~/myCode/0_Cpp11]$ g++ testJson.cpp -ljsoncpp -std=c++11
[edison@vm-centos:~/myCode/0_Cpp11]$ ./a.out 
序列化后的 JSON:
[
   {
      "key1" : "value1",
      "key2" : "value2"
   },
   {
      "key3" : "value3",
      "key4" : "value4"
   }
]


反序列化结果：
key1: value1
key2: value2
-----
key3: value3
key4: value4
-----

*/