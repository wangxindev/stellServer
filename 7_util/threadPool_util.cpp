#include "threadPool_util.h"


static std::mutex mtx_thread_pool_new_instance;
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
	bRun = true;
	_pool->run1th(this);
	if (th == NULL)
		th = new thread(&threadWrapper::thRunCallBack, this, data);
}

void threadWrapper::deleteSelf()
{
	bDeleteSelf = true;
}

bool threadWrapper::isRun()
{
	return bRun;
}

void threadWrapper::thRunCallBack(void * data)
{
	while (!bDeleteSelf)
	{
		while (bRun)
		{
			pRunCallBack(data);
			bRun = false;
			_pool->stop1th(this);
		}
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
			if (!_thList.empty())
			{
				th = _thList.back();
				_thList.pop_back();
			}
			else
			{
				th = _thRunList.back();
				_thList.pop_back();
			}
			th->deleteSelf();
		}
	}
}

void threadPool_util::runLogic(pRunCall callback, void*data)
{
	threadWrapper* th;
	if (!_thList.empty())
	{
		th = _thList.front();
		th->setRunFun(callback, data);
	}
	else
	{
		fun_w_t funWT;
		funWT.callback = callback;
		funWT.data = data;
		_logicList.push_back(funWT);
	}
}

threadPool_util* threadPool_util::getInstance()
{
	if (!_instance)
	{
		mtx_thread_pool_new_instance.lock();
		if (!_instance)
		{
			_instance = new threadPool_util;
			whileTh = new thread(&threadPool_util::whileRun, this);
		}
		mtx_thread_pool_new_instance.unlock();
	}
	return _instance;
}

void threadPool_util::run1th(threadWrapper* th)
{
	_thRunList.push_back(th);
	_thList.remove(th);
}

void threadPool_util::stop1th(threadWrapper* th)
{
	_thList.push_back(th);
	_thRunList.remove(th);
}

void threadPool_util::whileRun()
{
	while (true)
	{
		if (!_thList.empty() && !_logicList.empty())
		{
			threadWrapper* th;
			th = _thList.front();
			fun_w_t funWT = _logicList.back();
			th->setRunFun(funWT.callback, funWT.data);
			_logicList.pop_back();
		}
	}
}

