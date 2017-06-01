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
	
	 @brief	初始化线程池线程数量，可以重复调用，运行状态的线程,轻易不要使用，可能会导致某些正在运行的线程无法运行任务
	
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

 @brief	初始化线程池数量

 @author	wangxin
 @date	2017/6/1

 @param	thread_count	Number of threads.
 */

#define thread_pool_init(thread_count) threadPool_util::getInstance()->init(thread_count)

/**
 @def	thread_pool_get_one_thread(callback, userData)

 @brief	从线程池中获取一个线程执行任务

 @author	wangxin
 @date	2017/6/1

 @param	callback	一个void（void*）的回调函数.
 @param	userData	用户参数，会传递给回调函数
 */

#define thread_pool_get_one_thread(callback, userData) threadPool_util::getInstance()->runLogic(callback, userData)

#endif
