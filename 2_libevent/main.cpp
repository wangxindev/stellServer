#include <stdio.h>
#include <string.h>
#include "httpServer.h"
#include <iostream>


class logicTest :public logicBase
{
public:
	logicTest(char *logicInfo) :logicBase::logicBase(logicInfo) {}
	void logic_run(struct evhttp_request * req, void * userData)
	{
		//����Ҫʹ�õ�buffer����
		evbuffer* buf = evbuffer_new();
		if (!buf) {
			return;
		}
		//��ȡ�����URI
		const char* uri = (char*)evhttp_request_get_uri(req);
		//��Ӷ�Ӧ��HTTP����
		evbuffer_add_printf(buf, "<html>");
		evbuffer_add_printf(buf, "<head><title>MyHttpServer</title></head>");
		evbuffer_add_printf(buf, "<body>");
		//����URI��ʾ��ͬ��ҳ��
		if (strcmp(uri, "/test") == 0) {
			evbuffer_add_printf(buf, "<p>I love C++</p>");
		}
		evbuffer_add_printf(buf, "</body>");
		evbuffer_add_printf(buf, "</html>");
		//�ظ����ͻ���
		evhttp_send_reply(req, HTTP_OK, "OK", buf);
		evbuffer_free(buf);
		std::cout << "send stop\n";
	}
};

int main(void)
{
	logicTest test("test");

	httpServer server("0.0.0.0", 8000, 10, 1024, NULL);
	server.addInterfaceLogic(&test);
	server.startServer();

	return 0;
}