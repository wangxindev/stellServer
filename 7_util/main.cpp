#include <iostream>
#include "http_util.h"
#include "threadPool_util.h"
#include <windows.h>
#include <mutex>
#include <random>
#include <time.h>
using std::mutex;

using namespace std;
typedef std::function<void(void*)> pRunCall;
mutex mtx;
static unsigned int num = 0;
void callbk(void * data)
{
	int count = rand() % 10000;
	int a = 0;
	for (int i = 0; i < count; ++i)
		a = a + i;
	cout << "num=" << num++ << " index:" << *(int*)data << " thread id:" << std::this_thread::get_id() << " a=" << a  << " count=" << count << endl;
}
int main(void)
{
	srand((unsigned)time(0));
	cout << "main thread id : " << std::this_thread::get_id() << endl;

	thread_pool_init(8);
	for (static int i = 0; i < 1000; ++i)
	{
		thread_pool_get_one_thread(callbk, new int(i));
		//Sleep(100);
	}

	getchar();
	//system("pause");
	return 0;
}
