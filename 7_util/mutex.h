#pragma once
#ifndef __MUTEX_HELP_H__
#define __MUTEX_HELP_H__
#include <mutex>
class mutex_wx
{
public:
	mutex_wx() { mtx.lock(); }
	~mutex_wx() { mtx.unlock(); }
	std::mutex mtx;
};
#endif
