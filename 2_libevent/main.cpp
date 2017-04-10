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
		//创建要使用的buffer对象
		evbuffer* buf = evbuffer_new();
		if (!buf) {
			return;
		}
		//获取请求的URI
		const char* uri = (char*)evhttp_request_get_uri(req);
		//添加对应的HTTP代码
		evbuffer_add_printf(buf, "<html>");
		evbuffer_add_printf(buf, "<head><title>MyHttpServer</title></head>");
		evbuffer_add_printf(buf, "<body>");
		//根据URI显示不同的页面
		if (strcmp(uri, "/test") == 0) {
			evbuffer_add_printf(buf, "<p>I love C++</p>");
		}
		evbuffer_add_printf(buf, "</body>");
		evbuffer_add_printf(buf, "</html>");
		//回复给客户端
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