#include <iostream>
#include "logHelp.h"
#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

int main(void)
{
	while (1)
	{
		/*想要调整具体参数可以通过这种方式创建*/
		//logHelp::getRef()->createLog(log_config());
		//logcom->logOut(DEBUG_E, "main", "hello world\n");

		/*如果没有创建日志，则按照默认参数创建*/
		LOGE("test", "testmain");
		LOGE("info", "testinfo");//目前只支持字符串
#ifdef WIN32
		Sleep(1000);
#else
		sleep(1);
#endif
	}
	getchar();
	return 0;
}