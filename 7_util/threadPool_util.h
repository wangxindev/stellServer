#ifndef __THREAD_POOL_UTIL_H__
#define __THREAD_POOL_UTIL_H__
#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <list>
using std::list;
using std::thread;
using std::mutex;

class threadPool_util;
typedef std::function<void(void*)> pRunCall;

class threadWrapper
{
public:
	threadWrapper(threadPool_util* pool);
	void setRunFun(pRunCall runCall, void* data);
	void deleteSelf();
	bool isRun();

private:
	void thRunCallBack(void * data);
private:

	pRunCall pRunCallBack;
	thread * th;
	bool bRun;

	bool bDeleteSelf;

	threadPool_util* _pool;
	mutex m;
	std::condition_variable cv;
};

typedef struct fun_warpper
{
	pRunCall callback;
	void * data;
} fun_w_t;

class threadPool_util
{
	friend class threadWrapper;
public:
	~threadPool_util();

	/**
	 @fn	void threadPool_util::init(int thCount);
	
	 @brief	��ʼ���̳߳��߳������������ظ����ã�����״̬���߳�,���ײ�Ҫʹ�ã����ܻᵼ��ĳЩ�������е��߳��޷���������
	
	 @author	wangxin
	 @date	2017/6/1
	
	 @param	thCount	Number of ths.
	 */

	void init(int thCount);


	void runLogic(pRunCall callback, void*data = NULL);

	static threadPool_util* getInstance();
	

private:
	threadPool_util();

	void run1th(threadWrapper* th);
	void stop1th(threadWrapper* th);
	void whileRun();

	list<fun_w_t> _logicList;
	list<threadWrapper*> _thList;
	list<threadWrapper*> _thRunList;

	int _thCount;
	int _runThCount;

	static threadPool_util* _instance;

	thread * whileTh;
};

/**
 @def	thread_pool_init(thread_count)

 @brief	��ʼ���̳߳�����

 @author	wangxin
 @date	2017/6/1

 @param	thread_count	Number of threads.
 */

#define thread_pool_init(thread_count) threadPool_util::getInstance()->init(thread_count)

/**
 @def	thread_pool_get_one_thread(callback, userData)

 @brief	���̳߳��л�ȡһ���߳�ִ������

 @author	wangxin
 @date	2017/6/1

 @param	callback	һ��void��void*���Ļص�����.
 @param	userData	�û��������ᴫ�ݸ��ص�����
 */

#define thread_pool_get_one_thread(callback, userData) threadPool_util::getInstance()->runLogic(callback, userData)

#endif
