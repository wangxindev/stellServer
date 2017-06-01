#include <iostream>
#include "http_util.h"
#include "threadPool_util.h"
#include <windows.h>

using namespace std;
typedef std::function<void(void*)> pRunCall;
void callbk(void *)
{
	cout << "haha" << endl;
	cout << std::this_thread::get_id() << endl;
}
int main(void)
{

	threadPool_util::getInstance()->init(4);
	while (true)
	{
		threadPool_util::getInstance()->runLogic(callbk);
		Sleep(1000);
	}

	system("pause");
	return 0;
}
