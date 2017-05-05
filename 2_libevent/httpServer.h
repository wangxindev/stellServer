#ifndef __HTTPSERVER_H_
#define __HTTPSERVER_H_

#include <list>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/http.h>
#include <string>
#include <stdio.h>
#include <string.h>

#ifndef WIN32
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#endif
/**
 @class	logicBase

 @brief	ҵ���߼����࣬ҵ�������̳в���дlogic_run.

 @author	wangxin
 @date	2017/3/31
 */

class logicBase {
public:
	logicBase(char * info) { _info = info; }
	~logicBase() {}
	virtual void logic_run(struct evhttp_request *, void *) {}
	const char *getLogicInfo() { return _info; }

	std::string getHttpHeadParam(char * param, struct evhttp_request * req)
	{
		const char *uri = evhttp_request_get_uri(req);
		//decoded uri
		char *decoded_uri = evhttp_decode_uri(uri);
		//����URI�Ĳ���(��GET�����Ĳ���)
		struct evkeyvalq *params = evhttp_request_get_input_headers(req);
		evhttp_parse_query(decoded_uri, params);
		if (const char * str = evhttp_find_header(params, param))
		{ 
			return std::string(str);
		}
		return std::string();
	}

	std::string getPostHttpBody(struct evhttp_request * req)
	{
		//��ȡPOST����������
		std::string data;
		evbuffer * post_buff = evhttp_request_get_input_buffer(req);

		while (evbuffer_get_length(post_buff)) {
			int n;
			char cbuf[128] = { 0 };
			n = evbuffer_remove(post_buff, cbuf, sizeof(post_buff) - 1);
			data += cbuf;
		}
		return data;
	}
protected:
	char *_info;//ҵ����Ϣ���������Ϊ��õ�url��ҵ�����Դ�Ϊ�ָ��
};

//�ṹ�壬��װһ��������c�������ݶ���ָ��
class httpServer;
typedef struct suserData
{
	httpServer* self;
	void * userData;
}userData_t;

/**
 @class	httpServer

 @brief	http������ʵ��.���ﲻ����ip��֤.

 @author	wangxin
 @date	2017/3/31
 */

class httpServer
{
public:
private:
	typedef struct serverInfo
	{
		/** @brief	The IP. */
		char * ip;
		/** @brief	The port. */
		int port;
		/** @brief	�߳���. */
		int nthread;
		/** @brief	�ļ�����������. */
		int backlog;
		/** @brief	Information describing the user. */
		userData_t  userData;
	} serverInfo;

public:

	/**
	 @fn	httpServer::httpServer(char *ip, int port, void * userData);
	
	 @brief	����һ��http������.
	
	 @author	wangxin
	 @date	2017/3/31
	
	 @param [in]	ip			����������ip.
	 @param 			port		�������˿�.
	 @param [in,out]	userData	If non-null, information describing the user.
	 */

	httpServer(char *ip, int port, int nthread, int backlog, void * userData);
	~httpServer();

	inline void addInterfaceLogic(logicBase * logic) { if (logic != NULL) _interfaceLogicList.push_back(logic); }
	inline std::list <logicBase*> getInterfaceLogicList() { return _interfaceLogicList; }
	inline void addLogic(logicBase * logic) { if (_logic == NULL) _logic = logic; }
	inline logicBase* getLogic() { return _logic; }

	bool startServer();

	void send404Error(struct evhttp_request* req, void* arg);

private:

	serverInfo _info;
	std::list<logicBase*> _interfaceLogicList;//�ӿ����񼯺�
	logicBase* _logic;//����ҵ����
};

#endif

