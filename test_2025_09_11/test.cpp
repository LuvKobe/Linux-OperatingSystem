#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

//线程对象的构造方式

//一、调用无参的构造函数
//thread提供了无参的构造函数，调用无参的构造函数创建出来的线程对象没有关联任何线程函数，即没有启动任何线程。
//比如：

// void func(int n)
// {
// 	for (int i = 0; i <= n; i++)
// 	{
// 		cout << i << endl;
// 	}
// }

// int main()
// {
// 	thread t1;
// 	t1 = thread(func, 10);
// 	t1.join();

// 	return 0;
// }


//二、调用带参的构造函数
//调用带参的构造函数创建线程对象，能够将线程对象与线程函数fn进行关联。
// int main()
// {
// 	thread t2(func, 10);
// 	t2.join();

// 	return 0;
// }


//三、调用移动构造函数
//thread提供了移动构造函数，能够用一个右值线程对象来构造一个线程对象
// int main()
// {
// 	thread t3 = thread(func, 10);
// 	t3.join();

// 	return 0;
// }

/*
说明一下：

1、线程是操作系统中的一个概念，线程对象可以关联一个线程，用来控制线程以及获取线程的状态。

2、如果创建线程对象时没有提供线程函数，那么该线程对象实际没有对应任何线程。

3、如果创建线程对象时提供了线程函数，那么就会启动一个线程来执行这个线程函数，该线程与主线程一起运行。

4、thread类是防拷贝的，不允许拷贝构造和拷贝赋值，但是可以移动构造和移动赋值，
可以将一个线程对象关联线程的状态转移给其他线程对象，并且转移期间不影响线程的执行。
*/

//获取线程的id的方式
/*
调用thread的成员函数get_id可以获取线程的id，但该方法必须通过线程对象来调用get_id函数，
如果要在线程对象关联的线程函数中获取线程id，可以调用this_thread命名空间下的get_id函数。

比如：
*/

// void func()
// {
// 	cout << this_thread::get_id() << endl; // 获取线程id
// }

// int main()
// {
// 	thread t1(func);

// 	/*
// 	加上 join() 的作用：
// 	主线程会 等待 t1 执行完之后再退出；
// 	保证子线程的任务能够正常完成。

// 	如果你不想等怎么办？
// 	可以用 t1.detach() // 主线程不等，子线程“独立运行”
// 	这样主线程不等子线程了；
// 	但是你要确保子线程在主线程结束前跑完，否则就容易出错，比如访问已释放的资源（悬垂指针）。
// 	*/
// 	t1.join();
// 	return 0;
// }


//线程函数的参数问题
/*
线程函数的参数是以值拷贝的方式拷贝到线程栈空间中的，就算线程函数的参数为引用类型，
在线程函数中修改后也不会影响到外部实参，因为其实际引用的是线程栈中的拷贝，而不是外部实参。

比如：
*/

//join与detach
/*
启动一个线程后，当这个线程退出时，
需要对该线程所使用的资源进行回收，否则可能会导致内存泄露等问题。
thread库给我们提供了如下两种回收线程资源的方式：
*/

/*
join方式:
主线程创建新线程后，可以调用join函数等待新线程终止，当新线程终止时join函数就会自动清理线程相关的资源。

join函数清理线程的相关资源后，thread对象与已销毁的线程就没有关系了，
因此一个线程对象一般只会使用一次join，否则程序会崩溃。

但如果一个线程对象join后，又调用移动赋值函数，将一个右值线程对象的关联线程的状态转移过来了，
那么这个线程对象又可以调用一次join。

比如：

int main()
{
	thread t(func, 20);
	t.join();

	t = thread(func, 30);
	t.join();
	return 0;
}
*/

/*
但采用join的方式结束线程，在某些场景下也可能会出现问题。
比如在该线程被join之前，如果中途因为某些原因导致程序不再执行后续代码，这时这个线程将不会被join。
*/
// void func(int n)
// {
// 	for (int i = 0; i <= n; i++)
// 	{
// 		cout << i << endl;
// 	}
// }

// bool DoSomething()
// {
// 	return false;
// }

// int main()
// {
// 	thread t(func, 20);

// 	//...
// 	if (!DoSomething())
// 		return -1;
// 	//...

// 	t.join(); //不会被执行
// 	return 0;
// }


/*
因此采用join方式结束线程时，join的调用位置非常关键，
为了避免上述问题，可以采用RAII的方式对线程对象进行封装，
也就是利用对象的生命周期来控制线程资源的释放。

比如：
*/

// class myThread
// {
// public:
// 	myThread(thread& t)
// 		:_t(t)
// 	{}

// 	~myThread()
// 	{
// 		//joinable判断该线程是否已经执行完毕，如果是则返回true，否则返回false
// 		if (_t.joinable())
// 		{
// 			_t.join();
// 		}
// 	}

// 	//防拷贝
// 	myThread(myThread const&) = delete;
// 	myThread& operator=(const myThread&) = delete;
// private:
// 	thread& _t;
// };

/*
每当创建一个线程对象后，就用myThread类对其进行封装产生一个myThread对象。
当myThread对象生命周期结束时就会调用析构函数，
在析构中会通过joinable判断这个线程是否需要被join，
如果需要那么就会调用join对其该线程进行等待。
*/

// void func(int n)
// {
// 	for (int i = 0; i <= n; i++)
// 	{
// 		cout << i << endl;
// 	}
// }
// bool DoSomething()
// {
// 	return false;
// }

// //例如刚才的代码中，使用myThread类对线程对象进行封装后，就能保证线程一定会被join。
// int main()
// {
// 	thread t(func, 20);
// 	myThread mt(t); //使用myThread对线程对象进行封装

// 	//...
// 	if (!DoSomething())
// 		return -1;
// 	//...

// 	t.join();
// 	return 0;
// }


/*
主线程创建新线程后，也可以调用detach函数将新线程与主线程进行分离，分离后新线程会在后台运行，
其所有权和控制权将会交给C++运行库，此时C++运行库会保证当线程退出时，其相关资源能够被正确回收。

1、使用detach的方式回收线程的资源，一般在线程对象创建好之后就立即调用detach函数。

2、否则线程对象可能会因为某些原因，在后续调用detach函数分离线程之前被销毁掉，这时就会导致程序崩溃。

3、因为当线程对象被销毁时会调用thread的析构函数，而在thread的析构函数中会通过joinable判断这个线程是否需要被join，
如果需要那么就会调用terminate终止当前程序（程序崩溃）。
*/

//互斥量库mutex，在C++11中，mutex中总共包了四种互斥量

/*
1、std::mute
mutex锁是C++11提供的最基本的互斥量，mutex对象之间不能进行拷贝，也不能进行移动。

mutex中常用的成员函数如下：

成员函数	功能
lock	对互斥量进行加锁
try_lock	尝试对互斥量进行加锁
unlock	对互斥量进行解锁，释放互斥量的所有权


2、std::recursive_mutex
recursive_mutex叫做递归互斥锁，该锁专门用于递归函数中的加锁操作。

3、std::timed_mutex
timed_mutex中提供了以下两个成员函数。

4、std::recursive_timed_mutex
recursive_timed_mutex就是recursive_mutex和timed_mutex的结合，
recursive_timed_mutex既支持在递归函数中进行加锁操作，也支持定时尝试申请锁。
*/

//加锁示例
//在没有使用互斥锁保证线程安全的情况下，让两个线程各自打印1-100的数字，就会导致控制台输出错乱。比如：
// void func(int n)
// {
// 	for (int i = 1; i <= n; i++)
// 	{
// 		cout << i << endl;
// 	}
// }

// int main()
// {
// 	thread t1(func, 100);
// 	thread t2(func, 100);

// 	t1.join();
// 	t2.join();
// 	return 0;
// }

/*
如果要让两个线程的输出不会相互影响，即不会让某一次输出中途被另一个线程打断，
那么就需要用互斥锁对打印过程进行保护。
*/

//这里加锁的方式有两种，一种是在for循环体内进行加锁，一种是在for循环体外进行加锁。比如：

// void func(int n, mutex& mtx)
// {
// 	mtx.lock();
// 	for (int i = 1; i <= n; i++)
// 	{
// 		cout << i << " ";
// 	}
// 	cout << endl;
// 	mtx.unlock();
// }
// int main()
// {
// 	mutex mtx;
// 	thread t1(func, 100, ref(mtx));
// 	thread t2(func, 100, ref(mtx));

// 	t1.join();
// 	t2.join();
// 	return 0;
// }

/*
此处在for循环体外加锁比在for循环体内加锁更高效，因为在for循环体内加锁会导致线程打印数字时频繁进行加锁解锁操作，
而如果在for循环体外加锁，那么这两个线程只需要在开始打印1之前进行一次加锁，在打印完100后进行一次解锁就行了。

在for循环体外加锁也就意味着两个线程的打印过程变成了串行的，即一个线程打印完1-100后另一个线程再打印，
但这时打印效率提高了，因为避免了这两个线程间的频繁切换。

为了保证两个线程使用的是同一个互斥锁，线程函数必须以引用的方式接收传入的互斥锁，
并且在传参时需要使用ref函数保持对互斥锁的引用。

此外，也可以将互斥锁定义为全局变量，或是用lambda表达式定义线程函数，
然后以引用的方式将局部的互斥锁进行捕捉，这两种方法也能保证两个线程使用的是同一个互斥锁。
*/


/*
经验分享：
在项目中实际不太建议定义全局变量，因为全局变量如果定义在头文件中，
当这个头文件被多个源文件包含时，在这多个源文件中都会对这个全局变量进行定义，
这时就会导致变量重定义，但如果将全局变量定义为静态，那这个全局变量就只在当前文件可见。

如果确实有一些变量需要在多个文件中使用，那么一般建议将这些变量封装到一个类当中，
然后将这个类设计成单例模式，当需要使用这些变量时就通过这个单例对象去访问即可。
*/

/*
实现两个线程交替打印1-100:
尝试用两个线程交替打印1-100的数字，要求一个线程打印奇数，另一个线程打印偶数，并且打印数字从小到大依次递增。

该题目主要考察的就是线程的同步和互斥。
- 互斥：两个线程都在向控制台打印数据，为了保证两个线程的打印数据不会相互影响，因此需要对线程的打印过程进行加锁保护。
- 同步：两个线程必须交替进行打印，因此需要用到条件变量让两个线程进行同步，当一个线程打印完再唤醒另一个线程进行打印。

但如果只有同步和互斥是无法满足题目要求的:
- 首先，我们无法保证哪一个线程会先进行打印，不能说先创建的线程就一定先打印，后创建的线程先打印也是有可能的。
- 此外，有可能会出现某个线程连续多次打印的情况，
比如线程1先创建并打印了一个数字，当线程1准备打印第二个数字的时候线程2可能还没有创建出来，
或是线程2还没有在互斥锁上进行等待，这时线程1就会再次获取到锁进行打印。

鉴于此，这里还需要定义一个flag变量，该变量的初始值设置为true：
- 假设让线程1打印奇数，线程2打印偶数。
那么就让线程1调用wait函数阻塞等待时，传入的可调用对象返回flag的值，
而让线程2调用wait函数阻塞等待时，传入的可调用对象返回!flag的值。

- 由于flag的初始值是true，就算线程2先获取到互斥锁也不能进行打印，
因为最开始线程2调用wait函数时，会因为可调用对象的返回值为false而被阻塞，这就保证了线程1一定先进行打印。

- 为了让两个线程交替进行打印，因此两个线程每次打印后都需要更改flag的值，
线程1打印完后将flag的值改为false并唤醒线程2，
这时线程2被唤醒时其可调用对象的返回值就变成了false，这时线程2就可以进行打印了。

- 当线程2打印完后再将flag的值改为true并唤醒线程1，这时线程1就又可以打印了，
就算线程2想要连续打印也不行，因为如果线程1不打印，
那么线程2的可调用对象的返回值就一直为false，对于线程1也是一样的道理。
*/

// int main()
// {
// 	int n = 100;
// 	mutex mtx;
// 	//（条件变量）是 C++11 引入的一种线程同步机制，主要用于实现线程之间的等待与通知。
// 	// 它解决的是“一个线程等待某个条件成立，另一个线程通知它”的问题。
// 	condition_variable cv;
// 	bool flag = true;

// 	// 奇数
// 	thread t1([&]{
// 		int i = 1;
// 		while (i <= 100)
// 		{
// 			unique_lock<mutex> ul(mtx);
// 			cv.wait(ul, [&flag]()->bool{return flag;}); //等待条件变量满足
// 			cout << "线程 1: " <<this_thread::get_id() << ": " << i << endl;
// 			i += 2;
// 			flag = false;
// 			cv.notify_one();  //唤醒条件变量下等待的一个线程
// 		}
// 	});

// 	// 偶数
// 	thread t2([&]{
// 		int j = 2;
// 		while (j <= 100)
// 		{
// 			unique_lock<mutex> ul(mtx);
// 			cv.wait(ul, [&flag]()->bool{return !flag;}); //等待条件变量满足
// 			cout << "线程 2: " << this_thread::get_id() << ": " << j << endl;
// 			j += 2;
// 			flag = true;
// 			cv.notify_one();  //唤醒条件变量下等待的一个线程
// 		}
// 	});

// 	t1.join();
// 	t2.join();

// 	return 0;
// }

class OddEvenPrinter {
private:
    int max_num;
    bool flag; // true：奇数线程执行；false：偶数线程执行
    mutex mtx;
    condition_variable cv;

public:
    OddEvenPrinter(int n) 
		: max_num(n), 
		flag(true) 
	{}

    void printOdd() 
	{
        int i = 1;
        while (i <= max_num) 
		{
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [&]() { return flag; });  // 等待该自己执行
            cout << "线程 1: " << this_thread::get_id() << ": " << i << endl;
            i += 2;
            flag = false;
            cv.notify_one();
        }
    }

    void printEven() 
	{
        int j = 2;
        while (j <= max_num) 
		{
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [&]() { return !flag; });  // 等待该自己执行
            cout << "线程 2: " << this_thread::get_id() << ": " << j << endl;
            j += 2;
            flag = true;
            cv.notify_one();
        }
    }

    void run() 
	{
        thread t1(&OddEvenPrinter::printOdd, this);
        thread t2(&OddEvenPrinter::printEven, this);
        t1.join();
        t2.join();
    }
};

int main() {
    OddEvenPrinter printer(100);
    printer.run();
    return 0;
}
