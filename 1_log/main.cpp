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
		/*��Ҫ���������������ͨ�����ַ�ʽ����*/
		//logHelp::getRef()->createLog(log_config());
		//logcom->logOut(DEBUG_E, "main", "hello world\n");

		/*���û�д�����־������Ĭ�ϲ�������*/
		LOGE("test", "testmain");
		LOGE("info", "testinfo");//Ŀǰֻ֧���ַ���
#ifdef WIN32
		Sleep(1000);
#else
		sleep(1);
#endif
	}
	getchar();
	return 0;
}