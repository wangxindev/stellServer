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
	bRun = true;
	pRunCallBack = runCall;
	this->data = data;
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

bool threadWrapper::isRun()
{
	return bRun;
}

void threadWrapper::thRunCallBack()
{
	while (!bDeleteSelf)
	{
		static int in = 0;
		//std::cout << "self: " << in++ << std::endl;
		pRunCallBack(data);
		bRun = false;
		_pool->stop1th();
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
	_thNoRunSize = 0;

}

threadPool_util::~threadPool_util()
{
}

void threadPool_util::init(int thCount)
{
	_thNoRunSize = thCount;
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
				if (th = back())
					pop_back();
			}
			else
			{
				if (th = back())
					pop_back();
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
	if (_thNoRunSize > 0)
	{
		if (th = getNotRunTh())
		{
			run1th();
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

void threadPool_util::run1th()
{
	--_thNoRunSize;
}

void threadPool_util::stop1th()
{
	++_thNoRunSize;
	cv.notify_all();
}

void threadPool_util::pop_back()
{
	lock(__LINE__);
	_thList.pop_back();
	unlock(__LINE__);
}

void threadPool_util::remove(threadWrapper* th)
{
	lock(__LINE__);
	_thList.remove(th);
	unlock(__LINE__);
}

threadWrapper* threadPool_util::getNotRunTh()
{
	lock(__LINE__);
	if (_thNoRunSize > 0)
	{
		for (auto & th : _thList)
		{
			if (!th->isRun())
			{
				unlock(__LINE__);
				return th;
			}
		}
	}
	unlock(__LINE__);
	return NULL;
}

threadWrapper* threadPool_util::back()
{
	lock(__LINE__);
	if (_thList.size() > 0)
	{
		unlock(__LINE__);
		return _thList.back();
	}
	unlock(__LINE__);
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
		if (_instance!= NULL && _thNoRunSize > 0 && _logicList.size() > 0)
		{
			int num = _logicList.size();
			threadWrapper* th = NULL;
			if(th = getNotRunTh())
			{
				fun_w_t funWT = _logicList.front();
				run1th();
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

