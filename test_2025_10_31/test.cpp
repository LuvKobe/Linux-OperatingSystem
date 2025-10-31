#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <thread>
#include <mutex>
#include <initializer_list>
#include <condition_variable>

using namespace std;

// struct Point
// {
//     int _x;
//     int _y;
// };

// int main()
// {
//     int arr1[] = {1, 2, 3, 4, 5};
//     int arr2[5] = {0};
//     Point p = {1, 2};

//     cout << "arr1: ";
//     for (int i = 0; i < sizeof(arr1) / sizeof(arr1[0]); i++)
//     {
//         cout << arr1[i] << " ";
//     }
//     cout << endl;

//     cout << "arr2: ";
//     for (int i = 0; i < sizeof(arr2) / sizeof(arr2[0]); i++)
//     {
//         cout << arr2[i] << " ";
//     }
//     cout << endl;

//     cout << "Point: " << p._x << " " << p._y << endl;

//     return 0;
// }

// struct Point
// {
//     int _x;
//     int _y;
// };

// int main()
// {
//     int x1 = 1; // 可添加等号
//     int x2{2};  // 可不添加等号
//     cout << x1 << " " << x2 << endl;

//     int arr1[]{1, 2, 3, 4, 5};
//     int arr2[5]{0};

//     Point p{1, 2};
//     cout << p._x << " " << p._y << endl;

//     // C++11中列表初始化也可以用于new表达式中（C++98无法初始化）
//     int *p1 = new int[4]{0};
//     int *p2 = new int[5]{1, 2, 3, 4, 5};

//     return 0;
// }

// class Date
// {
// public:
//     //
//     Date(int year, int month, int day)
//         : _y(year), _m(month), _d(day)
//     {
//     }

//     //
//     ~Date()
//     {
//     }

// private:
//     int _y; // 年
//     int _m; // 月
//     int _d; // 日
// };

// int main()
// {
//     Date d1(2025, 10, 1); // 老的初始化方式

//     // C++11支持的列表初始化，这里会调用构造函数初始化
//     Date d2{2025, 10, 2};
//     Date d3 = {2025, 10, 3};

//     return 0;
// }

// int main()
// {
//     // the type of il is an initializer_list
//     auto il = {10, 20, 30};
//     cout << typeid(il).name() << endl;
//     return 0;
// }

// int main()
// {
//     vector<int> v = {1, 2, 3, 4};
//     list<int> lt = {1, 2};

//     // 这里{"sort", "排序"}会先初始化构造一个pair对象
//     map<string, string> dict = {{"sort", "排序"}, {"insert", "插入"}};

//     // 使用大括号对容器赋值
//     v = {10, 20, 30};

//     return 0;
// }

// // 注意包含头文件
// #include <initializer_list>

// class MyVector
// {
// public:
//     MyVector(initializer_list<int> list)
//     {
//         for (int v : list)
//             _data.push_back(v);
//     }

//     void print()
//     {
//         for (int v : _data)
//             cout << v << " ";
//         cout << "\n";
//     }

// private:
//     vector<int> _data;
// };

// int main()
// {
//     MyVector v = {1, 2, 3, 4};
//     v.print(); // 1 2 3 4

//     return 0;
// }

// void func(std::initializer_list<int> list)
// {
//     for (auto x : list)
//         cout << x << " ";
//     cout << endl;
// }

// int main()
// {
//     func({10, 20, 30});

//     return 0;
// }

#include <typeinfo>
#include <cstring>

// int main()
// {
//     int i = 10;
//     auto p = &i;
//     auto pf = strcpy;

//     cout << typeid(p).name() << endl;
//     cout << typeid(pf).name() << endl;

//     map<string, string> dict = {{"sort", "排序"}, {"insert", "插入"}};
//     // map<string, string>::iterator it = dict.begin();
//     auto it = dict.begin();

//     return 0;
// }

// decltype的一些使用使用场景
// template <class T1, class T2>
// void F(T1 t1, T2 t2)
// {
//     decltype(t1 * t2) ret; // 对于 F(1,'a')，这里是 int
//     cout << "F ret: " << typeid(ret).name() << endl;
// }

// int main()
// {
//     const int x = 1;
//     double y = 2.2;

//     decltype(x * y) ret; // double
//     decltype(&x) p;      // const int*

//     cout << "ret: " << typeid(ret).name() << endl;
//     cout << "p:   " << typeid(p).name() << endl;

//     F(1, 'a'); // 打印 int

//     return 0;
// }

// int main()
// {
//     vector<int> v = {1, 2, 3};
//     for (int x : v)
//     {
//         cout << x << " ";
//     }
//     cout << endl;

//     return 0;
// }

// int main()
// {
//     map<string, int> m = {{"A", 1}, {"B", 2}};

//     // 第一种
//     for (const auto &p : m)
//     {
//         cout << p.first << ":" << p.second << "\n";
//     }

//     return 0;
// }

// #include <iostream>
// #include <memory>
// using namespace std;

// class B;

// class A
// {
// public:
//     weak_ptr<B> b; // 不增加引用计数
// };


// int main()
// {
//     auto p1 = make_shared<int>(20);
//     auto p2 = p1;                   // 引用计数 +1
//     cout << p1.use_count() << endl; // 2

//     return 0;
// }

// int main()
// {
//     unique_ptr<int> p1(new int(10));
//     cout << *p1 << endl;

//     // unique_ptr 禁止拷贝 ✅只能移动
//     unique_ptr<int> p2 = move(p1);

//     return 0;
// }


#include <boost/algorithm/string.hpp>

int main()
{
    vector<string> result1;
    vector<string> result2;
    string target = "this is title\3\3\3\3this is content\3this is url";
    boost::split(result1, target, boost::is_any_of("\3"));
    boost::split(result2, target, boost::is_any_of("\3"), boost::token_compress_on); // \3只会压缩成1个

    cout << "result1: ";
    for (const auto &s : result1)
    {
        cout << s << endl;
    }

    cout << "result2: ";
    for (const auto &s : result2)
    {
        cout << s << endl;
    }


    return 0;
}