#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <thread>
#include <mutex>
#include <initializer_list>
#include <condition_variable>

using namespace std;


#include <typeinfo>
#include <cstring>
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