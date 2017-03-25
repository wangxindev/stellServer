#ifndef __HTTPSERVER_H_
#define __HTTPSERVER_H_

#include <list>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/http.h>

//ҵ���߼����࣬ҵ�������̳в���дlogic_run
class logicBase {
public:
	logicBase(char * info) { _info = info; }
	~logicBase() {}
	virtual void logic_run(struct evhttp_request *, void *) {}
	const char *getLogicInfo() { return _info; }
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

//http������ʵ��.���ﲻ����ip��֤
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
	std::list<logicBase*> _interfaceLogicList;//�ӿ����񼯺�
	logicBase* _logic;//����ҵ����
};

#endif

