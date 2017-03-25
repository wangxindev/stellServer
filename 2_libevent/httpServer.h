#ifndef __HTTPSERVER_H_
#define __HTTPSERVER_H_

#include <list>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/http.h>

//业务逻辑父类，业务类必须继承并复写logic_run
class logicBase {
public:
	logicBase(char * info) { _info = info; }
	~logicBase() {}
	virtual void logic_run(struct evhttp_request *, void *) {}
	const char *getLogicInfo() { return _info; }
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

//http服务器实现.这里不包含ip验证
class httpServer
{
public:
private:
	typedef struct serverInfo
	{
		char * ip;
		int port;
		userData_t  userData;
	} serverInfo;

public:
	httpServer(char *ip, int port, void * userData);
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

