#include <iostream>
#include <string>
#include <fstream>
using namespace std;

int main()
{
    ifstream in("./log.txt", ios::in | ios::binary);
    if (in.is_open()) cout << "open success" << endl;
    
    string s;
    while (getline(in, s))
    {
        cout << s << endl;
    }

    in.close();

    return 0;
}
