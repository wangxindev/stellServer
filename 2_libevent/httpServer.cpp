#include "httpServer.h"

int init_win_socket()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return -1;
	}
	return 0;
}

void HttpServerHandler(struct evhttp_request* req, void* arg)
{
    //获取请求的URI
    const char* uri = (char*)evhttp_request_get_uri(req);
	userData_t *data = (userData_t*)arg;

	for (auto logic : data->self->getInterfaceLogicList())
	{
		std::string str("/");
		str.append(logic->getLogicInfo());
		if (str.compare(uri) == 0)
		{
			logic->logic_run(req, data->userData);
			return;
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

httpServer::httpServer(char * ip, int port, void * userData_)
{
	_logic = NULL;
	memset(&_info, '0', sizeof(serverInfo));
	_info.ip = ip;
	_info.port = port;
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

bool httpServer::startServer()
{

	//创建event_base和evhttp
    event_base* base = event_base_new();
    evhttp* http_server = evhttp_new(base);
    if (!http_server) {
        return false;
    }
    //绑定到指定地址上
    int ret = evhttp_bind_socket(http_server, _info.ip, _info.port & 0xFFFF);
    if (ret != 0) {
        return false;
    }
    //设置事件处理函数
    evhttp_set_gencb(http_server, HttpServerHandler, &_info.userData);

    //启动事件循环，当有http请求的时候会调用指定的回调
    event_base_dispatch(base);
    evhttp_free(http_server);
    return true;
}

void httpServer::send404Error(evhttp_request * req, void * arg)
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
		evbuffer_add_printf(buf, "<head><title>HttpServer</title></head>");
		evbuffer_add_printf(buf, "<body>");
		//根据URI显示不同的页面
		evbuffer_add_printf(buf, "<p>404</p>");
		evbuffer_add_printf(buf, "</body>");
		evbuffer_add_printf(buf, "</html>");
		//回复给客户端
		evhttp_send_reply(req, HTTP_NOTFOUND, "notfound", buf);
		evbuffer_free(buf);
}

