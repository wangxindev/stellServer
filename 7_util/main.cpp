#include <iostream>
#include "http_util.h"
#include "threadPool_util.h"
#include <windows.h>
#include <mutex>
#include <random>
#include <sstream>
#include <time.h>
using std::mutex;
template<class T>
static string toString(T data)
{
	std::stringstream sstr;
	string str;
	sstr << data;
	sstr >> str;
	return str;
}

using namespace std;
typedef std::function<void(void*)> pRunCall;
mutex mtx;
static unsigned int num = 0;
string str;
void callbk(void * data)
{
#if 1
	int count = rand() % 10000;
	int a = 0;
	for (int i = 0; i < count; ++i)
		a = a + i;
	mtx.lock();
	str.append(toString(num));
	cout << "num=" << num++ << " index:" << *(int*)data << " thread id:" << std::this_thread::get_id() << " a=" << a  << " count=" << count << endl;
	mtx.unlock();
#else
	mtx.lock();
	cout <<"index:" << *(int*)data << " thread id:" << std::this_thread::get_id() << endl;
	mtx.unlock();
	Sleep(100);
#endif

}
int main(void)
{
	srand((unsigned)time(0));
	cout << "main thread id : " << std::this_thread::get_id() << endl;

	thread_pool_init(8);
	for (static int i = 0; i < 10000; ++i)
	{
		thread_pool_get_one_thread(callbk, new int(i));
	}

	getchar();
	//system("pause");
	return 0;
}
