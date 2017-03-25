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
    //��ȡ�����URI
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

	//�������·�����ǽӿ���ִ������ҵ��
	if (data->self->getLogic() != NULL)
	{
		data->self->getLogic()->logic_run(req, data->userData);
		return;
	}

	//���ǽӿڷ��ʣ��ֲ�������ҵ������һ��404����
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

	//����event_base��evhttp
    event_base* base = event_base_new();
    evhttp* http_server = evhttp_new(base);
    if (!http_server) {
        return false;
    }
    //�󶨵�ָ����ַ��
    int ret = evhttp_bind_socket(http_server, _info.ip, _info.port & 0xFFFF);
    if (ret != 0) {
        return false;
    }
    //�����¼�������
    evhttp_set_gencb(http_server, HttpServerHandler, &_info.userData);

    //�����¼�ѭ��������http�����ʱ������ָ���Ļص�
    event_base_dispatch(base);
    evhttp_free(http_server);
    return true;
}

void httpServer::send404Error(evhttp_request * req, void * arg)
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
		evbuffer_add_printf(buf, "<head><title>HttpServer</title></head>");
		evbuffer_add_printf(buf, "<body>");
		//����URI��ʾ��ͬ��ҳ��
		evbuffer_add_printf(buf, "<p>404</p>");
		evbuffer_add_printf(buf, "</body>");
		evbuffer_add_printf(buf, "</html>");
		//�ظ����ͻ���
		evhttp_send_reply(req, HTTP_NOTFOUND, "notfound", buf);
		evbuffer_free(buf);
}

