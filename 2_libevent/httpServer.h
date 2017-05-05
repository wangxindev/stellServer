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

 @brief	业务逻辑父类，业务类必须继承并复写logic_run.

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
		//解析URI的参数(即GET方法的参数)
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
		//获取POST方法的数据
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
	char *_info;//业务信息，可以理解为获得的url，业务类以此为分割点
};

//结构体，包装一下用来向c函数传递对象指针
class httpServer;
typedef struct suserData
{
	httpServer* self;
	void * userData;
}userData_t;

/**
 @class	httpServer

 @brief	http服务器实现.这里不包含ip验证.

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
		/** @brief	线程数. */
		int nthread;
		/** @brief	文件描述符数量. */
		int backlog;
		/** @brief	Information describing the user. */
		userData_t  userData;
	} serverInfo;

public:

	/**
	 @fn	httpServer::httpServer(char *ip, int port, void * userData);
	
	 @brief	创建一个http服务器.
	
	 @author	wangxin
	 @date	2017/3/31
	
	 @param [in]	ip			服务器监视ip.
	 @param 			port		服务器端口.
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
	std::list<logicBase*> _interfaceLogicList;//接口任务集合
	logicBase* _logic;//正常业务处理
};

#endif

