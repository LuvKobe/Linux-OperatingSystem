#include <iostream>
#include "contacts.pb.h"

int main()
{
    std::string people_str;

    // 对⼀个联系⼈的信息使⽤PB进⾏序列化，并将结果打印出来。
    {
        contacts::PeopleInfo people;
        people.set_name("李四");
        people.set_age(18);
        // 调⽤序列化方法，将序列化后的二进制序列存入string中
        if (!people.SerializeToString(&people_str))
        {
            std::cerr << "序列化联系人失败！" << std::endl;
            return -1;
        }
        // 走到这里, 序列化成功, 打印结果
        std::cout << "序列化成功, 结果为: " << people_str << std::endl;
    }

    // 对序列化后的内容使⽤PB进⾏反序列，解析出联系⼈信息并打印出来。
    {
        contacts::PeopleInfo people;
        // 调用反序列化方法，读取string中存放的二进制序列，并反序列化出对象
        if (!people.ParseFromString(people_str))
        {
            std::cerr << "反序列化出联系⼈失败！" << std::endl;
            return -1;
        }
        // 走到这里, 反序列化成功, 打印结果
        std::cout << "反序列化成功！" << std::endl;
        std::cout << "姓名: " << people.name() << std::endl;
        std::cout << "年龄: " << people.age() << std::endl;

    }

    return 0;
}
