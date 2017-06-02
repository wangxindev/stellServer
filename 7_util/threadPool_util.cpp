#include "threadPool_util.h"
#include <iostream>


static std::mutex mtx_thread_pool_new_instance;
std::mutex mtx_thread_pool_list;
std::mutex mtx_thread_pool_main;
std::mutex mtx_thread_pool_while;
threadWrapper::threadWrapper(threadPool_util* pool)
{
	_pool = pool;
	bRun = false;
	th = NULL;
	bDeleteSelf = false;
}


void threadWrapper::setRunFun(pRunCall runCall, void *data)
{
	pRunCallBack = runCall;
	this->data = data;
	//std::cout << "thread id :" << std::this_thread::get_id() << " data=" << *(int*)data << std::endl;
	cv.notify_all();
	if (th == NULL)
	{
		th = new thread(&threadWrapper::thRunCallBack, this);
		th->detach();
	}
}

void threadWrapper::deleteSelf()
{
	bDeleteSelf = true;
}

void threadWrapper::thRunCallBack()
{
	while (!bDeleteSelf)
	{
		pRunCallBack(data);
		_pool->stop1th(this);
		std::unique_lock<mutex> ulock(m);
		cv.wait(ulock);
	}
}


//--------------------------------------------------------------------------
threadPool_util* threadPool_util::_instance = NULL;

threadPool_util::threadPool_util()
{
	_thCount = 0;
	_runThCount = 0;

}

threadPool_util::~threadPool_util()
{
}

void threadPool_util::init(int thCount)
{
	static bool bone = true;
	if (bone && _instance != NULL)
	{
		bone = false;
		whileTh = new thread(&threadPool_util::whileRun, _instance);
		whileTh->detach();
	}

	this->_thCount = thCount;
	int th_count = _thList.size();
	if (thCount >= th_count)
	{
		int create_count = thCount - th_count;
		for (int i = 0; i < create_count; ++i)
			_thList.push_back(new threadWrapper(this));
	}
	else
	{
		int deleteCount = th_count - thCount;
		for (; deleteCount > 0; --deleteCount)
		{
			threadWrapper* th;
			if (_thList.size() > 0)
			{
				if (th = back(false))
					pop_back(false);
			}
			else
			{
				if (th = back(true))
					pop_back(true);
			}
			if (th)
				th->deleteSelf();
		}
	}
}

void threadPool_util::runLogic(pRunCall callback, void*data)
{
	mtx_thread_pool_main.lock();
	cv.notify_all();
	threadWrapper* th;
	if (_thList.size() > 0)
	{
		if (th = front(false))
		{
			run1th(th);
			th->setRunFun(callback, data);
		}
	}
	else
	{
		fun_w_t funWT;
		funWT.callback = callback;
		funWT.data = data;
		_logicList.push_back(funWT);
	}
	mtx_thread_pool_main.unlock();
}

threadPool_util* threadPool_util::getInstance()
{
	if (!_instance)
	{
		mtx_thread_pool_new_instance.lock();
		if (!_instance)
		{
			_instance = new threadPool_util;
		}
		mtx_thread_pool_new_instance.unlock();
	}
	return _instance;
}

void threadPool_util::run1th(threadWrapper* th)
{
	_thRunList.push_back(th);
	remove(false, th);
}

void threadPool_util::stop1th(threadWrapper* th)
{
	remove(true, th);
	_thList.push_back(th);
	cv.notify_all();
}

void threadPool_util::pop_back(bool isRunList)
{
	lock(__LINE__);
	if (isRunList)
		_thRunList.pop_back();
	else
		_thList.pop_back();
	unlock(__LINE__);
}

void threadPool_util::pop_front(bool isRunList)
{
	mtx_thread_pool_list.lock();
	lock(__LINE__);
	if (isRunList)
		_thRunList.pop_front();
	else
		_thList.pop_front();
	unlock(__LINE__);
}

void threadPool_util::remove(bool isRunList, threadWrapper* th)
{
	lock(__LINE__);
	if (isRunList)
		_thRunList.remove(th);
	else
		_thList.remove(th);
	unlock(__LINE__);
}

threadWrapper* threadPool_util::front(bool isRunList)
{
	lock(__LINE__);
	if (isRunList)
	{
		if (_thRunList.size() > 0)
		{
			unlock(__LINE__);
			return _thRunList.front();
		}
	}
	else
	{
		if (_thList.size() > 0)
		{
			unlock(__LINE__);
			return _thList.front();
		}
	}
	unlock(__LINE__);
	return NULL;
}

threadWrapper* threadPool_util::back(bool isRunList)
{
	lock(__LINE__);
	if (isRunList)
	{
		if (_thRunList.size() > 0)
		{
			unlock(__LINE__);
			return _thRunList.back();
		}
	}
	else
	{
		if (_thList.size() > 0)
		{
			unlock(__LINE__);
			return _thList.back();
		}
	}
	return NULL;
}

void threadPool_util::lock(int line)
{
#if 0
	if (line != -1)
		std::cout << "lock   :" << line << " ->thread id :"<< std::this_thread::get_id() << std::endl;
#endif
	mtx_thread_pool_list.lock();
}

void threadPool_util::unlock(int line)
{
#if 0
	if (line != -1)
		std::cout << "unlock   :" << line << " ->thread id :"<< std::this_thread::get_id() << std::endl;
#endif
	mtx_thread_pool_list.unlock();
}

void threadPool_util::whileRun()
{
	while (true)
	{
		mtx_thread_pool_while.lock();
		if (_instance!= NULL && _thList.size() > 0 && _logicList.size() > 0)
		{
			int num = _logicList.size();
			//printf("logic size = %d\n", num);
			threadWrapper* th = NULL;
			if(th = front(false))
			{
				fun_w_t funWT = _logicList.front();
				run1th(th);
				th->setRunFun(funWT.callback, funWT.data);
				_logicList.pop_front();
			}
		}
		else
		{
			std::unique_lock<mutex> ulock(m);
			cv.wait(ulock);
		}
		mtx_thread_pool_while.unlock();
	}
}

