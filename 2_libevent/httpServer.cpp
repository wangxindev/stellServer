#include "httpServer.h"
#include <thread>
#include <iostream>
#include <vector>
#include <exception>

int init_win_socket()
{
#ifdef WIN32
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return -1;
	}
#endif
	return 0;
}

void HttpServerHandler(struct evhttp_request* req, void* arg)
{
	userData_t *data = (userData_t*)arg;
	try
	{
#ifdef DEBUG
		std::cout << "=======================================thread id:" << std::this_thread::get_id() << std::endl;
#endif
		//获取请求的URI
		const char* uri = (char*)evhttp_request_get_uri(req);

		for (auto logic : data->self->getInterfaceLogicList())
		{
			std::string str("/");
			str.append(logic->getLogicInfo());
			char uriBuf[1024] = { 0 };
			if (strlen(uri) >= str.size() && strlen(uri) < 1023 && str.size() < 1023)
			{
				strncpy(uriBuf, uri, str.size());
				if (str.compare(uriBuf) == 0)
				{
					logic->logic_run(req, data->userData);
					return;
				}
			}
		}

		//如果请求路径不是接口则执行正常业务
		if (data->self->getLogic() != NULL)
		{
			data->self->getLogic()->logic_run(req, data->userData);
			return;
		}

		//不是接口访问，又不是正常业务，则发送一个404错误
		data->self->send404Error(req, data->userData);
	}
	catch (std::exception  * e)
	{
		data->self->send404Error(req, data->userData);
		if (e != NULL)
		{
			std::cout << "查找业务错误:" << e->what() << std::endl;
		}
	}
}

httpServer::httpServer(char * ip, int port, int nthread, int backlog, void * userData_)
{
	_logic = NULL;
	memset(&_info, '0', sizeof(serverInfo));
	_info.ip = ip;
	_info.port = port;
	_info.nthread = nthread;
	_info.backlog = backlog;
	_info.userData.userData = userData_;
	_info.userData.self = this;
#ifdef WIN32
	init_win_socket();
#endif
}

httpServer::~httpServer()
{
#ifdef WIN32
	WSACleanup();
#endif
}

int httpserver_bindsocket(int port, int backlog) {
	int r;
	int nfd;
	nfd = socket(AF_INET, SOCK_STREAM, 0);
	if (nfd < 0) return -1;

	int one = 1;
	r = setsockopt(nfd, SOL_SOCKET, SO_REUSEADDR, (char *)&one, sizeof(int));

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	r = bind(nfd, (struct sockaddr*)&addr, sizeof(addr));
	if (r < 0) return -1;
	r = listen(nfd, backlog);
	if (r < 0) return -1;

#ifdef WIN32
	u_long ul = 1;
	ioctlsocket(nfd, FIONBIO, &ul);
#else
	int flags;
	if ((flags = fcntl(nfd, F_GETFL, 0)) < 0
		|| fcntl(nfd, F_SETFL, flags | O_NONBLOCK) < 0)
		return -1;
#endif

	return nfd;
}

bool httpServer::startServer()
{
	int nfd = httpserver_bindsocket(_info.port & 0xFFFF, _info.backlog);
	if (nfd < 0) return false;
	for (int i = 0; i < _info.nthread; ++i)
	{
		struct event_base * base = event_base_new();
		struct evhttp * http_server = evhttp_new(base);
		if (!http_server)
		{
			return false;
		}

		int r = evhttp_accept_socket(http_server, nfd);
		if (r != 0) return false;
		evhttp_set_gencb(http_server, HttpServerHandler, &_info.userData);
		std::thread th(event_base_dispatch, base);
		th.detach();
	}
	printf("http server start OK! thread num:%d\n", _info.nthread);
	while (1)
	{
#ifdef WIN32
		Sleep(10000);
#else
		sleep(10);
#endif
	}
	return true;
}

void httpServer::send404Error(evhttp_request * req, void * arg)
{
	const char* uri = (char*)evhttp_request_get_uri(req);
	if (uri != NULL)
	{
		std::cout << "uri:" << uri << std::endl;
	}

	//创建要使用的buffer对象
	evbuffer* buf = evbuffer_new();
	if (!buf) {
		return;
	}
	//获取请求的URI
	const char* uri = (char*)evhttp_request_get_uri(req);
	//添加对应的HTTP代码
	evbuffer_add_printf(buf, "<html>");
	evbuffer_add_printf(buf, "<head><title>HttpServer</title></head>");
	evbuffer_add_printf(buf, "<body>");
	//根据URI显示不同的页面
	evbuffer_add_printf(buf, "<p>404</p>");
	evbuffer_add_printf(buf, "</body>");
	evbuffer_add_printf(buf, "</html>");
	//回复给客户端
	evhttp_send_reply(req, HTTP_NOTFOUND, "notfound", buf);
	evbuffer_free(buf);
	std::cout << "send stop\n";
}

