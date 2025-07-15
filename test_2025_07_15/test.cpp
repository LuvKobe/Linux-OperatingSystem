#include <iostream>
#include <cstdio>

using namespace std;

// // 交换两个整型
// void Swap(int& x, int& y)
// {
// 	int tmp = x;
// 	x = y;
// 	y = tmp;
// }
// // 交换两个双精度浮点型
// void Swap(double& x, double& y)
// {
// 	double tmp = x;
// 	x = y;
// 	y = tmp;
// }

// template<typename T>
// void Swap(T& x, T& y)
// {
//     T tmp = x;
//     x = y;
//     y = tmp;
// }

// int main()
// {
//     int a = 10, b = 20;
//     Swap(a, b);
//     printf("a=%d, b=%d\n", a, b);

//     double x = 1.1, y = 2.2;
//     Swap(x, y);
//     printf("x=%.1lf, y=%.1lf\n", x, y);

//     return 0;
// }

// template<typename T>
// T add(const T& x, const T& y)
// {
//     return x + y;
// }

// int main()
// {
//     // int a = 10, b = 20;
//     // cout << add(a, b) << endl;

//     int a = 10;
//     double b = 1.1; //如果传入的参数类型与模板参数类型不匹配，编译器会尝试进行隐式类型转换
//     int c = add<int>(a, b); 

//     cout << c << endl;

//     return 0;
// }


// template<typename T>
// T Add(const T& x, const T& y)
// {
// 	return x + y;
// }

// int main()
// {
// 	int a = Add(2, 2.2); //模板函数不允许自动类型转换，不能通过编译
// 	return 0;
// }


template<class T>
class Score1
{
public:
    Score1()
        : _Math(10),
        _Chinese(20),
        _English(30)
    {}

    void Print()
    {
        cout << "数学:" << _Math << endl;
		cout << "语文:" << _Chinese << endl;
		cout << "英语:" << _English << endl;
    }
private:
    T _Math;
    T _Chinese;
    T _English;
};

//类模板中的成员函数若是放在类外定义时，需要加模板参数列表。
template<class T>
class Score2
{
public:
    Score2()
        :_Math(50.5),
        _Chinese(60.5),
        _English(70.5)
    {}

	void Print();
private:
	T _Math;
	T _Chinese;
	T _English;
};

//类模板中的成员函数在类外定义，需要加模板参数列表
template<class T>
void Score2<T>::Print()
{
	cout << "数学:" << _Math << endl;
	cout << "语文:" << _Chinese << endl;
	cout << "英语:" << _English << endl;
}

int main()
{
    Score1<int> s1;
    s1.Print();

    cout << endl;

    Score2<double> s2;
    s2.Print();
    return 0;
}