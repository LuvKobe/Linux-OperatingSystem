#include <iostream>
#include <string>
using namespace std;

/*
多态就是函数调用的多种形态，使用多态能够使得不同的对象去完成同一件事时，产生不同的动作和结果。

例如，在现实生活当中，普通人买票是全价，学生买票是半价，而军人允许优先买票。
不同身份的人去买票，所产生的行为是不同的，这就是所谓的多态。

在继承中要想构成多态需要满足两个条件：
- 必须通过基类的指针或者引用调用虚函数。
- 被调用的函数必须是虚函数，且派生类必须对基类的虚函数进行重写。
*/

// 被virtual修饰的类成员函数被称为虚函数。
class Person
{
public:
	//被virtual修饰的类成员函数
	virtual void BuyTicket()
	{
		cout << "买票-全价" << endl;
	}
};
/*
需要注意的是：
- 只有类的非静态成员函数前可以加virtual，普通函数前不能加virtual。
- 虚函数这里的virtual和虚继承中的virtual是同一个关键字，但是它们之间没有任何关系。
  虚函数这里的virtual是为了实现多态，而虚继承的virtual是为了解决菱形继承的数据冗余和二义性。
*/


int main()
{
  ;
  return 0;
}
